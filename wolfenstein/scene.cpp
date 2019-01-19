//#define OPENEXR_DLL
//#define ALEMBIC_DLL

#include "scene.hpp"

#include <assert.h>
#include <Alembic/AbcCoreFactory/IFactory.h>
#include <Alembic/AbcCoreLayer/Read.h>

namespace wolf {
	scene::scene()
	{
	}

	scene::~scene()
	{
	}

	void scene::load_abc(const std::string & path, bool invert, std::vector<polymesh*>  & meshVec)
	{

		Alembic::AbcGeom::IArchive archive(Alembic::AbcCoreOgawa::ReadArchive(), path);
		//std::cout << "Reading: " << archive.getName() << std::endl;
		visit_object(archive, meshVec);
	}

	void scene::process_geometry(Alembic::AbcGeom::IObject &iObj, std::vector<polymesh*>  & meshVec)
	{

		polymesh* tmp_mesh = nullptr;

		Alembic::AbcGeom::IPolyMesh shape(iObj, Alembic::AbcGeom::kWrapExisting);
		Alembic::AbcGeom::IPolyMeshSchema &mesh = shape.getSchema();

		std::cout << mesh.getMetaData().serialize() << std::endl;
		
		assert(mesh.getUVsParam() != 0);
		Alembic::AbcGeom::IV2fGeomParam uv = mesh.getUVsParam();
		std::cout << uv.getMetaData().serialize() << std::endl;

		assert(mesh.getNormalsParam() != 0);
		Alembic::AbcGeom::IN3fGeomParam N = mesh.getNormalsParam();
		std::cout << N.getMetaData().serialize() << std::endl;

		//ACCESS ARBITRARY GEOM ATTRIBUTES THROUGH THIS

		assert(mesh.getArbGeomParams() != 0);

		//VECTOR
		auto P2 = Alembic::AbcGeom::IN3fGeomParam(mesh.getArbGeomParams(), "P2");

		//INT
		auto P3 = Alembic::AbcGeom::IInt16GeomParam(mesh.getArbGeomParams(), "test");

		//STRING
		auto P4 = Alembic::AbcGeom::IStringGeomParam(mesh.getArbGeomParams(), "yalla");

		std::cout << P2.getMetaData().serialize() << std::endl;
		std::cout << P3.getMetaData().serialize() << std::endl;
		std::cout << P4.getMetaData().serialize() << std::endl;

		//transformation matrix 

		Alembic::AbcGeom::M44d mat;
		glm::quat rot;

		get_final_matrix(iObj, mat, rot);

		//glm::mat4 rotMat = glm::toMat4(rot); 

		Alembic::AbcGeom::IPolyMeshSchema::Sample mesh_samp;
		mesh.get(mesh_samp);

		//User properties
		Alembic::AbcGeom::ICompoundProperty userProp = mesh.getUserProperties();
		

		tmp_mesh = new polymesh;

		if (userProp.valid())
		{
			for (size_t i = 0; i < userProp.getNumProperties(); ++i)
			{
				Alembic::AbcGeom::PropertyHeader propHeader = userProp.getPropertyHeader(i);
				std::string test = get_property_string(userProp, propHeader);
				std::cout << "property " << test << std::endl;
			}
		}

		Alembic::AbcGeom::IN3fGeomParam::Sample nsamp;
		Alembic::AbcGeom::IV2fGeomParam::Sample uvsamp;
		Alembic::AbcGeom::IN3dGeomParam::Sample psamp;
		//Alembic::AbcGeom::IN3dGeomParam::Sample p2samp;

		bool indexedNormals = N.isIndexed();
		//std::cout << "nsamp.isIndexed() " << indexedNormals << "\n";
		bool indexedUvs = uv.isIndexed();
		//std::cout << "uvsamp.isIndexed() " << indexedUvs << "\n";

		bool NisConstant = N.isConstant();
		//std::cout << "N.isConstant() " << NisConstant << "\n";

		if (NisConstant)
			nsamp = N.getIndexedValue();

		if (uv.isConstant())
			uvsamp = uv.getIndexedValue();

		//N3fArraySamplePtr nptr = N.getExpandedValue().getVals();
		//V2fArraySamplePtr uvptr = uv.getExpandedValue().getVals();

		//tmp containers
		std::vector<unsigned int> tmpVertIndices;
		std::vector<unsigned int> tmpUvIndices;
		std::vector<unsigned int> tmpNormIndices;
		std::vector<unsigned int> tmpFaceCounts;

		std::vector<glm::vec3> tmpVertices;
		//std::vector<glm::vec3> tmpP2;
		std::vector<glm::vec3> tmpNormals;
		std::vector<glm::vec2> tmpUvs;


		///////////////////////////////////////////////////////////////////////////
		//////////////////////////////VERTICES/////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////

		Alembic::AbcGeom::Int32ArraySamplePtr findicesPtr = mesh_samp.getFaceIndices();
		Alembic::AbcGeom::Int32ArraySamplePtr facesPtr = mesh_samp.getFaceCounts();


		for (auto i = 0; i < findicesPtr->size(); ++i)
		{

			tmpVertIndices.push_back(findicesPtr->get()[i]);
			//std::cout<<"vertex indices: "<<findicesPtr->get()[i]<<std::endl;
		}

		for (auto i = 0; i < facesPtr->size(); ++i)
		{
			tmpFaceCounts.push_back(facesPtr->get()[i]);
			//std::cout<<"face counts: "<<facesPtr->get()[i]<<std::endl;
		}


		Alembic::AbcGeom::P3fArraySamplePtr fpointPtr = mesh_samp.getPositions();

		for (auto i = 0; i < fpointPtr->size(); ++i)
		{
			Alembic::AbcGeom::V3f vertex = fpointPtr->get()[i];
			vertex *= mat;
			tmpVertices.push_back(glm::vec3(vertex.x, vertex.y, vertex.z));
			//std::cout<<"vertex: "<<fpointPtr->get()[i].x<<"\t"<<fpointPtr->get()[i].y<<"\t"<<fpointPtr->get()[i].z<<std::endl;
		}


		////////////////////////////////////////////////////////////////////////////
		///////////////////////////NORMALS//////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////


		for (auto i = 0; i < nsamp.getVals()->size(); ++i)
		{
			//glm::vec3 n = glm::vec3(0);
			glm::vec3 normal = glm::vec3((float)nsamp.getVals()->get()[i].x,
				(float)nsamp.getVals()->get()[i].y,
				(float)nsamp.getVals()->get()[i].z);
			//I am getting some issues when the rotation angle is null
			//glm::vec3 n = glm::gtx::quaternion::rotate(rot,normal);

			tmpNormals.push_back(normal);
		}


		Alembic::AbcGeom::UInt32ArraySamplePtr normIndices = nsamp.getIndices();

		for (auto i = 0; i < normIndices->size(); ++i)
		{
			tmpNormIndices.push_back(normIndices->get()[i]);
		}


		/////////////////////////////////////////////////////////////////////////////
		/////////////////////////////UVS/////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////

		Alembic::AbcGeom::UInt32ArraySamplePtr uvIndices = uvsamp.getIndices();

		for (auto i = 0; i < uvIndices->size(); ++i)
		{
			tmpUvIndices.push_back(uvIndices->get()[i]);
		}


		for (auto i = 0; i < uvsamp.getVals()->size(); ++i)
		{
			tmpUvs.push_back(glm::vec2(uvsamp.getVals()->get()[i].x, uvsamp.getVals()->get()[i].y));
		}

		////////////////////////DEBUG/////////////////////////////////////
		/*std::cout << "face count " << facesPtr->size() << std::endl;
		std::cout << "vert indices " << findicesPtr->size() << std::endl;
		std::cout << "normal indices " << normIndices->size() << std::endl;
		std::cout << "uv indices " << uvIndices->size() << std::endl;
		std::cout << "vertices " << fpointPtr->size() << std::endl;
		std::cout << "uvs " << uvsamp.getVals()->size() << std::endl;
		std::cout << "normals " << nsamp.getVals()->size() << std::endl;*/


		assert(tmp_mesh->is_valid(tmpVertIndices,tmpFaceCounts));
		assert(tmp_mesh->is_valid(tmpNormIndices,tmpFaceCounts));
		assert(tmp_mesh->is_valid(tmpUvIndices,tmpFaceCounts));

		//Alembic operates on a CW winding order whereas openGL does on a CCW basis
		tmp_mesh->swap_winding_order(tmpVertIndices,tmpFaceCounts);
		tmp_mesh->swap_winding_order(tmpUvIndices,tmpFaceCounts);
		tmp_mesh->swap_winding_order(tmpNormIndices,tmpFaceCounts);


		tmp_mesh->set_position(tmp_mesh->unindexed(tmpVertices, tmpVertIndices));
		tmp_mesh->set_normals(tmp_mesh->unindexed(tmpNormals, tmpNormIndices));
		tmp_mesh->set_uvs(tmp_mesh->unindexed(tmpUvs, tmpUvIndices));
		tmp_mesh->set_indices(tmp_mesh->vertices());


		meshVec.push_back(tmp_mesh);
	}

	void scene::test(const std::string& iFileName)
	{

		Alembic::AbcGeom::IArchive archive(Alembic::AbcCoreOgawa::ReadArchive(), iFileName);
		Alembic::AbcGeom::IObject topObject = archive.getTop();
		const Alembic::AbcGeom::MetaData md = topObject.getMetaData();
		std::cout << "Top Object is: " << md.serialize() << std::endl;
		std::cout << "Number of children: " << topObject.getNumChildren() << std::endl;

		for (int i = 0; i < topObject.getNumChildren(); i++)
		{
			Alembic::AbcGeom::IObject child(topObject, topObject.getChildHeader(i).getName());
			//NAMES
			std::cout << "child header is : " << child.getHeader().getFullName() << std::endl;
			std::cout << "child object name is : " << topObject.getChild(i).getName() << std::endl;
			//HANDLE TO XFORM
			const Alembic::AbcGeom::MetaData &md = child.getMetaData();
			std::cout << "child type is : " << md.serialize() << std::endl;

			for (int x = 0; x < child.getNumChildren(); x++)
			{
				Alembic::AbcGeom::IObject child2(child, child.getChildHeader(x).getName());
				const Alembic::AbcGeom::MetaData &md2 = child2.getMetaData();
				std::cout << "child2 type is : " << md2.serialize() << std::endl;
				
				Alembic::AbcGeom::IPolyMesh shape(child2, Alembic::AbcGeom::kWrapExisting);
				Alembic::AbcGeom::IPolyMeshSchema &mesh = shape.getSchema();

				std::cout << "serialize " << mesh.getMetaData().serialize() << std::endl;
				Alembic::AbcGeom::ICompoundProperty userProp = mesh.getUserProperties();
				std::cout << "user properties: " << mesh.getName() << std::endl;
				//std::cout << "num properties " << mesh.b << std::endl;
				
				auto it = mesh.getPositionsProperty().getName().begin();
				while (it != mesh.getPositionsProperty().getName().end())
				{
					std::cout << *it << std::endl;
					++it;

				}
			}

		}


	}

	void scene::visit_object(Alembic::AbcGeom::IArchive &archive, std::vector<polymesh*> &meshVec)
	{

		Alembic::AbcGeom::IObject topObject = archive.getTop();
		//std::cout << numChildren << std::endl;

		const Alembic::AbcGeom::MetaData &md = topObject.getMetaData();
		//Archive Meta Data
		//std::cout << md.serialize() << std::endl;

		for (int i = 0; i < topObject.getNumChildren(); i++)
		{
			Alembic::AbcGeom::IObject child(topObject, topObject.getChildHeader(i).getName());
			//NAMES
			//std::cout << "child header is : " << child.getHeader().getFullName() << std::endl;
			//std::cout << "child object name is : " << topObject.getChild(i).getName() << std::endl;
			//HANDLE TO XFORM
			const Alembic::AbcGeom::MetaData &md = child.getMetaData();
			//std::cout << "child type is : " << md.serialize() << std::endl;

			//ACTUAL POLYGONS
			for (int x = 0; x < child.getNumChildren(); x++)
			{
				Alembic::AbcGeom::IObject child2(child, child.getChildHeader(x).getName());
				const Alembic::AbcGeom::MetaData &md2 = child2.getMetaData();
				//std::cout << "child2 type is : " << md2.serialize() << std::endl;

				if (Alembic::AbcGeom::IPolyMeshSchema::matches(md2))
				{
					Alembic::AbcGeom::IPolyMesh mesh(child2, Alembic::AbcGeom::kWrapExisting);
					//std::cout << "Found Some Polygon mesh!  " << child2.getFullName() << std::endl;
					process_geometry(child2, meshVec);

				}
			}

		}
	}

	void scene::visit_object(Alembic::AbcGeom::IObject &iObj)
	{

		const Alembic::AbcGeom::MetaData &md = iObj.getMetaData();

		// only bother with meshes
		if (Alembic::AbcGeom::IPolyMeshSchema::matches(md))
		{
			Alembic::AbcGeom::IPolyMesh mesh(iObj, Alembic::AbcGeom::kWrapExisting);
			//std::cout << "Found Some Polygons!  " << iObj.getFullName() << std::endl;

		}

		// now the child objects
		for (size_t i = 0; i < iObj.getNumChildren(); i++)
		{
			visit_object(Alembic::AbcGeom::IObject(iObj, iObj.getChildHeader(i).getName()));
		}
	}

	void scene::accum_xform(Alembic::AbcGeom::M44d &xf, glm::quat &frot, Alembic::AbcGeom::IObject obj)
	{


		if (Alembic::AbcGeom::IXform::matches(obj.getHeader()))
		{
			Alembic::AbcGeom::IXform x(obj, Alembic::AbcGeom::kWrapExisting);
			Alembic::AbcGeom::XformSample xs;
			x.getSchema().get(xs);

			//for rotations 
			Alembic::AbcGeom::V3f axis = xs.getAxis();
			//this is in degrees
			float angle = xs.getAngle();
			//std::cout << "angle: " << angle << std::endl;
			Alembic::AbcGeom::V3f scale = xs.getScale();

			glm::vec3 normalizedAxis = (glm::vec3(axis.x, axis.y, axis.z) != glm::vec3(0)) ? glm::normalize(glm::vec3(axis.x, axis.y, axis.z)) : glm::vec3(axis.x, axis.y, axis.z);

			glm::quat rot = glm::angleAxis((float)angle, normalizedAxis);

			// AbcGeom::XformSample::getMatrix() will compute an Imath::M44d that
			// represents the condensed xform for this AbcGeom::XformSample.
			xf *= xs.getMatrix();
			frot = (axis.length() == 0) ? frot : frot * rot;
		}
	}

	//-*****************************************************************************
	void scene::get_final_matrix(Alembic::AbcGeom::IObject &iObj, Alembic::AbcGeom::M44d& mat, glm::quat& quat)
	{

		mat.makeIdentity();

		Alembic::AbcGeom::IObject parent = iObj.getParent();

		// Once the Archive's Top Object is reached, IObject::getParent() will
		// return an invalid IObject, and that will evaluate to False.
		while (parent)
		{
			accum_xform(mat, quat, parent);
			parent = parent.getParent();
		}

	}

#include <sstream>


	std::string scene::get_property_string(Alembic::AbcGeom::ICompoundProperty & parent, const Alembic::AbcGeom::PropertyHeader &propHeader)
	{

		std::ostringstream buffer;

		//NOTE: only supporting expected types here
		//real code would need to support more types
		//and animation, etc

		if (propHeader.isScalar())
		{
			if (Alembic::AbcGeom::IStringProperty::matches(propHeader))
			{
				Alembic::AbcGeom::IStringProperty prop(parent,
					propHeader.getName());
				buffer << propHeader.getName() << ": " << prop.getValue();

			}
			else if (Alembic::AbcGeom::IFloatProperty::matches(propHeader))
			{
				Alembic::AbcGeom::IFloatProperty prop(parent,
					propHeader.getName());

				buffer << propHeader.getName() << ": " << prop.getValue();


			}
		}

		return buffer.str();

	}


	void scene::walk_from_node(Alembic::AbcMaterial::MaterialFlatten & mafla,
		const std::string & nodeName,
		std::set<std::string> & alreadyWalked)
	{

		if (alreadyWalked.find(nodeName) != alreadyWalked.end())
		{
			return;
		}
		alreadyWalked.insert(nodeName);

		Alembic::AbcMaterial::MaterialFlatten::NetworkNode node = mafla.getNetworkNode(nodeName);
		if (!node.valid())
		{
			return;
		}

		std::string target("<undefined>"), nodeType("<undefined>");

		node.getTarget(target);
		node.getNodeType(nodeType);

		std::cout << "      NODE: " << node.getName() << std::endl;
		std::cout << "        target: " << target << std::endl;
		std::cout << "        nodeType: " << nodeType << std::endl;
		std::cout << "        parameters:" << std::endl;

		Alembic::AbcMaterial::MaterialFlatten::ParameterEntryVector parameters;
		node.getParameters(parameters);

		for (Alembic::AbcMaterial::MaterialFlatten::ParameterEntryVector::iterator I =
			parameters.begin(); I != parameters.end(); ++I)
		{
			std::cout << "           name: " << (*I).name << " ";
			std::cout << get_property_string((*I).parent, *(*I).header) << std::endl;
		}

		std::vector<std::string> connectedNodeNames;

		Alembic::AbcMaterial::MaterialFlatten::NetworkNode::ConnectionVector connections;
		node.getConnections(connections);

		std::cout << "        connections:" << std::endl;

		for (Alembic::AbcMaterial::MaterialFlatten::NetworkNode::ConnectionVector::iterator I =
			connections.begin(); I != connections.end(); ++I)
		{
			std::cout << "           input: " << (*I).inputName;
			std::cout << ", node: " << (*I).connectedNodeName << ", output: ";
			std::cout << (*I).connectedOutputName << std::endl;

			connectedNodeNames.push_back((*I).connectedNodeName);
		}

		for (std::vector<std::string>::iterator I = connectedNodeNames.begin();
			I != connectedNodeNames.end(); ++I)
		{

			walk_from_node(mafla, (*I), alreadyWalked);
		}

	}


	void scene::print_flattened_mafla(Alembic::AbcMaterial::MaterialFlatten & mafla)
	{
		std::vector<std::string> targetNames;

		mafla.getTargetNames(targetNames);

		if (!targetNames.empty())
		{
			std::cout << "monolithic targets: " << std::endl;
			for (std::vector<std::string>::iterator I = targetNames.begin();
				I != targetNames.end(); ++I)
			{
				const std::string & targetName = (*I);

				std::cout << "  " << targetName << std::endl;

				std::vector<std::string> shaderTypeNames;

				mafla.getShaderTypesForTarget(
					targetName, shaderTypeNames);

				for (std::vector<std::string>::iterator I =
					shaderTypeNames.begin(); I != shaderTypeNames.end(); ++I)
				{
					const std::string & shaderTypeName = (*I);

					std::string shaderName("<undefined>");

					mafla.getShader(targetName, shaderTypeName,
						shaderName);

					std::cout << "    " << shaderTypeName << ": ";
					std::cout << shaderName << std::endl;

					Alembic::AbcMaterial::MaterialFlatten::ParameterEntryVector paramEntries;

					mafla.getShaderParameters(targetName, shaderTypeName,
						paramEntries);

					for (Alembic::AbcMaterial::MaterialFlatten::ParameterEntryVector::iterator I =
						paramEntries.begin(); I != paramEntries.end(); ++I)
					{
						Alembic::AbcMaterial::MaterialFlatten::ParameterEntry & propEntry = (*I);

						const Alembic::AbcGeom::PropertyHeader &propHeader =
							*(propEntry.header);

						Alembic::AbcGeom::ICompoundProperty & params = propEntry.parent;

						std::cout << "      " << propEntry.name;
						std::cout << std::endl;

						std::cout << "        " << get_property_string(params, propHeader) << std::endl;

					}
				}
			}
		}

		mafla.getNetworkTerminalTargetNames(targetNames);

		if (!targetNames.empty())
		{
			std::cout << "network terminals: " << std::endl;
			for (std::vector<std::string>::iterator I = targetNames.begin();
				I != targetNames.end(); ++I)
			{
				const std::string & targetName = (*I);

				std::cout << "  " << targetName << std::endl;

				std::vector<std::string> shaderTypeNames;

				mafla.getNetworkTerminalShaderTypesForTarget(
					targetName, shaderTypeNames);

				for (std::vector<std::string>::iterator I =
					shaderTypeNames.begin(); I != shaderTypeNames.end(); ++I)
				{
					const std::string & shaderTypeName = (*I);

					std::string nodeName("<undefined>");
					std::string outputName("<undefined>");

					mafla.getNetworkTerminal(targetName, shaderTypeName,
						nodeName, outputName);

					std::cout << "    " << shaderTypeName << ": ";
					std::cout << "NODE: " << nodeName << ", OUTPUT: " << outputName << std::endl;

					std::set<std::string> alreadyWalkedNodes;


					walk_from_node(mafla, nodeName, alreadyWalkedNodes);


				}

			}

		}

	}


	void scene::print_flattened_material(Alembic::AbcMaterial::IMaterial & material)
	{
		std::cout << "---------------------------\n";
		std::cout << material.getFullName() << std::endl;
		std::cout << "---------------------------\n";

		Alembic::AbcMaterial::MaterialFlatten mafla(material);

		print_flattened_mafla(mafla);
	}


	void scene::print_material_schema(Alembic::AbcMaterial::IMaterialSchema & schema)
	{
		Alembic::AbcMaterial::MaterialFlatten mafla(schema);

		print_flattened_mafla(mafla);
	}
}