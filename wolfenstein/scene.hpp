#pragma once
#include "polymesh.hpp"

#include <string>
#include <vector>
#include <set>

#include <glm/glm.hpp>

#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreOgawa/All.h>
#include <Alembic/Abc/ErrorHandler.h>
#include <Alembic/AbcMaterial/all.h>
#include <glm/gtc/quaternion.hpp>


namespace wolf
{

	class scene
	{
	public:
		scene();
		~scene();
		void load_abc(const std::string & path, bool invert, std::vector<polymesh*>  & meshVec);

		void walk_from_node(Alembic::AbcMaterial::MaterialFlatten & mafla, const std::string & nodeName, std::set<std::string> & alreadyWalked);
		void print_flattened_mafla(Alembic::AbcMaterial::MaterialFlatten & mafla);
		std::string get_property_string(Alembic::AbcGeom::ICompoundProperty & parent, const Alembic::AbcGeom::PropertyHeader &propHeader);
		void print_flattened_material(Alembic::AbcMaterial::IMaterial & material);
		void print_material_schema(Alembic::AbcMaterial::IMaterialSchema & schema);
		void accum_xform(Alembic::AbcGeom::M44d &xf, glm::quat &frot, Alembic::AbcGeom::IObject obj);
		void get_final_matrix(Alembic::AbcGeom::IObject &iObj, Alembic::AbcGeom::M44d& mat, glm::quat& quat);
		void visit_object(Alembic::AbcGeom::IArchive &archive, std::vector<polymesh*> &meshVec);

		void visit_object(Alembic::AbcGeom::IObject &iObj);
		void process_geometry(Alembic::AbcGeom::IObject &iObj, std::vector<polymesh*> &meshVec);
		void test(const std::string& path);

	};
}