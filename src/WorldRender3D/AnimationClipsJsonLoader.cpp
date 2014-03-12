#include "AnimationClipsJsonLoader.h"
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\json_parser.hpp>
#include "AnimationClips.h"
#include <iostream>

using namespace std;
using namespace boost;
namespace xihad { namespace render3d
{
	AnimationClips AnimationClipsJsonLoader::load( const std::string& path )
	{
		property_tree::ptree treeData;

		try
		{
			read_json(path, treeData);
			return AnimationClips::loadFromPtree(treeData);
		}
		catch (property_tree::json_parser::json_parser_error& error)
		{
			cerr << error.what() << endl;
		}

		return AnimationClips();
	}

}}

