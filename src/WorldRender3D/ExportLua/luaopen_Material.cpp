#include <luaT/luaT.h>
#include <irrlicht/SMaterial.h>
#include <CppBase/StringUtil.h>
#include <iostream>
#include <Engine/SColor.h>

using namespace std;
using namespace irr;
using namespace video;
using namespace luaT;
namespace xihad { namespace render3d
{
	luaT_static void setMaterialType(SMaterial& mat, const char* type)
	{
		int idx = StringUtil::select(type, sBuiltInMaterialTypeNames);
		if (idx != -1)
			mat.MaterialType = (E_MATERIAL_TYPE) idx;
		else 
			cerr << "Unrecognized material type: "  << type << endl;
	}}

	luaT_static void setLighting(SMaterial& mat, bool b)
	{
		mat.Lighting = b;
	}}

	luaT_static void setZWriteEnable(SMaterial& mat, bool b)
	{
		mat.ZWriteEnable = b;
	}}
	
	luaT_static void setWireFrame(SMaterial& mat, bool b)
	{
		mat.Wireframe = b;
	}}

	luaT_static void setBackfaceCulling(SMaterial& mat, bool b)
	{
		mat.BackfaceCulling = b;
	}}

	static int setColorMaterial(lua_State* L)
	{
		static const char* const modes[] = { "none", "diffuse", "ambient", 
			"emissive", "specular", "diffuseAndAmbient", 0 };

		luaT_variable(L, 1, SMaterial&, mat);
		luaT_variable(L, 2, const char*, mode);
		int idx = StringUtil::select(mode, modes);
		if (idx != -1)
			mat.ColorMaterial = (E_COLOR_MATERIAL) idx;
		else
			luaL_argerror(L, 2, "Unrecognized color material mode");

		return 0;
	}

	static int setZBufferCompareFunc(lua_State* L)
	{
		static const char* const funcs[] = {
			"false", "<=", "==", "<", "!=", ">=", ">", "true", 0
		};
		luaT_variable(L, 1, SMaterial&, mat);
		
		if (lua_isboolean(L, 2))
		{
			luaT_variable(L, 2, bool, pass);
			if (pass)
				mat.ZBuffer = ECFN_NEVER;
			else 
				mat.ZBuffer = ECFN_ALWAYS;
		}
		else if (lua_isstring(L, 2))
		{
			luaT_variable(L, 2, const char*, func);
			int idx = StringUtil::select(func, funcs);

			if (idx != -1)
				mat.ZBuffer = (E_COMPARISON_FUNC) idx;
			else
				luaL_argerror(L, 2, "Unrecognized Z comparion func");
		}
		else
		{
			luaL_typerror(L, 2, "boolean/string");
		}

		return 0;
	}

	luaT_static void setBlend(SMaterial& mat, const char* fsrc, const char* fdst, const char* bops)
	{
		static const char* sBlendFactors[] = {
			"0", "1", "dst.rgb", "1-dst.rgb", "src.rgb", "1-src.rgb",
			"src.alpha", "1-src.alpha", "dst.alpha", "1-dst.alpha", 0
		};
		static const char* sBlendOperation[] = {
			"none", "add", "substract", "rev_substract", "min", "max", 0
		};

		int src = StringUtil::select(fsrc, sBlendFactors, " \t\n");
		int dst = StringUtil::select(fdst, sBlendFactors, " \t\n");
		if (src != -1 && dst != -1)
			mat.MaterialTypeParam = pack_textureBlendFunc((E_BLEND_FACTOR) src, (E_BLEND_FACTOR) dst);
		else if (src == -1)
			cerr << "Unrecognized src blend factor: " << fsrc << endl;
		else 
			cerr << "Unrecognized dst blend factor: " << fdst << endl;

		int ops = StringUtil::select(bops, sBlendOperation);
		if (ops != -1)
			mat.BlendOperation = (E_BLEND_OPERATION) ops;
		else 
			cerr << "Unrecognized blend operation: " << bops << endl;
	}}

	int luaopen_Material(lua_State* L)
	{
		luaT_defRegsBgn(smat)
			// TODO ColorMask
			luaT_mnamed_readattr(SMaterial, AmbientColor),
			luaT_mnamed_readattr(SMaterial, DiffuseColor),
			luaT_mnamed_readattr(SMaterial, SpecularColor),
			luaT_mnamed_readattr(SMaterial, EmissiveColor),
			luaT_mnamed_writeattr(SMaterial, AmbientColor),
			luaT_mnamed_writeattr(SMaterial, DiffuseColor),
			luaT_mnamed_writeattr(SMaterial, SpecularColor),
			luaT_mnamed_writeattr(SMaterial, EmissiveColor),
			luaT_lnamedfunc(setColorMaterial),
			luaT_lnamedfunc(setZBufferCompareFunc),
			luaT_cnamedfunc(setMaterialType),
			luaT_cnamedfunc(setBlend),
			luaT_cnamedfunc(setLighting),
			luaT_cnamedfunc(setWireFrame),
			luaT_cnamedfunc(setBackfaceCulling),
			luaT_cnamedfunc(setZWriteEnable),
		luaT_defRegsEnd
		MetatableFactory<SMaterial>::create(L, smat);
		return 0;
	}
}}

