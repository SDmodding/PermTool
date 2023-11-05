#pragma once

#define RESOURCE_TYPE_ActionTreeResource					0x4BCE8537
#define RESOURCE_TYPE_AlphaState							0x1BCFF4D5
#define RESOURCE_TYPE_Animation								0x8ACF9964
#define RESOURCE_TYPE_AnimationGroupResource				0x3D0EBC72
#define RESOURCE_TYPE_AudioFXSettings						0xAF8870AB
#define RESOURCE_TYPE_BIGFile								0x2C5C40A8
#define RESOURCE_TYPE_BIGFileNameLookup						0x164013D5
#define RESOURCE_TYPE_BSP									0x4F05B59A
#define RESOURCE_TYPE_BSPDebugData							0xE2C5C78C
#define RESOURCE_TYPE_BeamSettings							0x80EF0B08
#define RESOURCE_TYPE_BlendTreeResource						0xE691BB97
#define RESOURCE_TYPE_BonePalette							0x982456DB
#define RESOURCE_TYPE_Buffer								0x7A971479
#define RESOURCE_TYPE_BufferD3DResource						0x45E061F6
#define RESOURCE_TYPE_ChunkFileFatIndex						0xE445B80C
#define RESOURCE_TYPE_ChunkFileIndex						0x7040F7D2
#define RESOURCE_TYPE_Cloud									0x06526B66
#define RESOURCE_TYPE_CloudScene							0xD49B8DA4
#define RESOURCE_TYPE_CollisionInstance						0xA0B2CC13
#define RESOURCE_TYPE_CollisionMeshBundle					0xBD226A08
#define RESOURCE_TYPE_CoronaFlareSettings					0x9D6378CC
#define RESOURCE_TYPE_CoverData								0x5DEB3457
#define RESOURCE_TYPE_DecalSettings							0xDCAEC503
#define RESOURCE_TYPE_DynamicCoverData						0xE5150CC0
#define RESOURCE_TYPE_DynamicCoverGroupBundle				0x7117991B
#define RESOURCE_TYPE_DynamicLightGroupSettings				0x230C8A9C
#define RESOURCE_TYPE_DynamicLightSettings					0x8D0E8333
#define RESOURCE_TYPE_EffectEmitterSettings					0xD9B10F14
#define RESOURCE_TYPE_FXForceSettings						0x77554FC5
#define RESOURCE_TYPE_FXSettings							0x12289ADB
#define RESOURCE_TYPE_FarGroundLayout						0xF40E78D9
#define RESOURCE_TYPE_FlareSettings							0x83574C18
#define RESOURCE_TYPE_Font									0x2A1BE612
#define RESOURCE_TYPE_GeoSettings							0x52A8963A
#define RESOURCE_TYPE_ImposterGroup							0xAEDF1081
#define RESOURCE_TYPE_LightGroup							0x7480E00F
#define RESOURCE_TYPE_LightningSettings						0xB4AEE124
#define RESOURCE_TYPE_Locators								0x15506061
#define RESOURCE_TYPE_Material								0xF5F8516F
#define RESOURCE_TYPE_MaterialTable							0xEB9FE716
#define RESOURCE_TYPE_ModelData								0x6DF963B3
#define RESOURCE_TYPE_MorphTargets							0x02CD0C47
#define RESOURCE_TYPE_MovieResourceData						0xE9453F67
#define RESOURCE_TYPE_NISSpatialData						0xC762C801
#define RESOURCE_TYPE_NavMeshData							0xDD3C7B19
#define RESOURCE_TYPE_ParkourContainer						0xBDE53ECA
#define RESOURCE_TYPE_ParkourContainerBundle				0x12D3A53D
#define RESOURCE_TYPE_ParkourInstance						0xC31501A5
#define RESOURCE_TYPE_ParticleEmitterSettings				0xD05B6976
#define RESOURCE_TYPE_PropertySet							0x5B9BF81E
#define RESOURCE_TYPE_RasterState							0xB27A4B38
#define RESOURCE_TYPE_ReflectResource						0x616A903F
#define RESOURCE_TYPE_ReflectionGroup						0xD53B5BAC
#define RESOURCE_TYPE_Rig									0x1418DD74
#define RESOURCE_TYPE_RigInfoResource						0x036C2E8E
#define RESOURCE_TYPE_RigInstance							0x5C66C6BD
#define RESOURCE_TYPE_RoadNetwork							0x94132761
#define RESOURCE_TYPE_SceneLayer							0xE7F23AEE
#define RESOURCE_TYPE_SceneryGroup							0x7480E00B
#define RESOURCE_TYPE_ScreenParticleEmitterSettings			0x657192D6
#define RESOURCE_TYPE_SectionEffects						0x89A7BDF7
#define RESOURCE_TYPE_SectionLayout							0x3E50F7D5
#define RESOURCE_TYPE_ShaderBinary							0x985BE50C
#define RESOURCE_TYPE_ShaderTemplate						0x0C46AEEF
#define RESOURCE_TYPE_Sidewalk								0x2C81C14B
#define RESOURCE_TYPE_StateBlock							0xAF015A94
#define RESOURCE_TYPE_SymbolTableResource					0xE4868DBE
#define RESOURCE_TYPE_TerrainData							0xC462DD28
#define RESOURCE_TYPE_Texture								0xCDBFA090
#define RESOURCE_TYPE_TextureD3DResource					0x501B8E62
#define RESOURCE_TYPE_TrackStripSettings					0x86DE69F6
#define RESOURCE_TYPE_TrueCrowdDataBase						0x90EEF023
#define RESOURCE_TYPE_UELFragmentTable						0x32890C01
#define RESOURCE_TYPE_UILocalization						0x90CE6B7A
#define RESOURCE_TYPE_UIMinimapTile							0x9F34FF46
#define RESOURCE_TYPE_UIScreen								0x442A39D9
#define RESOURCE_TYPE_UniqueUIDTableResource				0x2C40FA26
#define RESOURCE_TYPE_VertexDecl							0xF7FC6B2D
#define RESOURCE_TYPE_VolumetricEffectSettings				0xA8EB0D0C
#define RESOURCE_TYPE_WeightSetGroupResource				0x1146D4C8
#define RESOURCE_TYPE_XMLFile								0x24D0C3A0
#define RESOURCE_TYPE_ZoneLayout							0x43FF83A9

struct ResourceTypeInfo_t
{
	const char* m_Name;
	const char* m_Icon;

	ResourceTypeInfo_t() { }
	ResourceTypeInfo_t(const char* p_Name, const char* p_Icon)
	{
		m_Name = p_Name;
		m_Icon = p_Icon;
	}
};

static std::unordered_map<uint32_t, ResourceTypeInfo_t> g_ResourceTypeMap =
{
	{ RESOURCE_TYPE_ActionTreeResource,				{ "ActionTreeResource",					u8"\u0100" }},
	{ RESOURCE_TYPE_AlphaState,						{ "AlphaState",							u8"\u0100" }},
	{ RESOURCE_TYPE_Animation,						{ "Animation",							u8"\u0100" }},
	{ RESOURCE_TYPE_AnimationGroupResource,			{ "AnimationGroupResource",				u8"\u0100" }},
	{ RESOURCE_TYPE_AudioFXSettings,				{ "AudioFXSettings",					u8"\u0100" }},
	{ RESOURCE_TYPE_BIGFile,						{ "BIGFile",							u8"\u0100" }},
	{ RESOURCE_TYPE_BIGFileNameLookup,				{ "BIGFileNameLookup",					u8"\u0100" }},
	{ RESOURCE_TYPE_BSP,							{ "BSP",								u8"\u0100" }},
	{ RESOURCE_TYPE_BSPDebugData,					{ "BSPDebugData",						u8"\u0100" }},
	{ RESOURCE_TYPE_BeamSettings,					{ "BeamSettings",						u8"\u0100" }},
	{ RESOURCE_TYPE_BlendTreeResource,				{ "BlendTreeResource",					u8"\u0100" }},
	{ RESOURCE_TYPE_BonePalette,					{ "BonePalette",						u8"\u0103" }},
	{ RESOURCE_TYPE_Buffer,							{ "Buffer",								u8"\u0104" }},
	{ RESOURCE_TYPE_BufferD3DResource,				{ "BufferD3DResource",					u8"\u0100" }},
	{ RESOURCE_TYPE_ChunkFileFatIndex,				{ "ChunkFileFatIndex",					u8"\u0100" }},
	{ RESOURCE_TYPE_ChunkFileIndex,					{ "ChunkFileIndex",						u8"\u0100" }},
	{ RESOURCE_TYPE_Cloud,							{ "Cloud",								u8"\u0100" }},
	{ RESOURCE_TYPE_CloudScene,						{ "CloudScene",							u8"\u0100" }},
	{ RESOURCE_TYPE_CollisionInstance,				{ "CollisionInstance",					u8"\u0100" }},
	{ RESOURCE_TYPE_CollisionMeshBundle,			{ "CollisionMeshBundle",				u8"\u0100" }},
	{ RESOURCE_TYPE_CoronaFlareSettings,			{ "CoronaFlareSettings",				u8"\u0100" }},
	{ RESOURCE_TYPE_CoverData,						{ "CoverData",							u8"\u0100" }},
	{ RESOURCE_TYPE_DecalSettings,					{ "DecalSettings",						u8"\u0100" }},
	{ RESOURCE_TYPE_DynamicCoverData,				{ "DynamicCoverData",					u8"\u0100" }},
	{ RESOURCE_TYPE_DynamicCoverGroupBundle,		{ "DynamicCoverGroupBundle",			u8"\u0100" }},
	{ RESOURCE_TYPE_DynamicLightGroupSettings,		{ "DynamicLightGroupSettings",			u8"\u0100" }},
	{ RESOURCE_TYPE_DynamicLightSettings,			{ "DynamicLightSettings",				u8"\u0100" }},
	{ RESOURCE_TYPE_EffectEmitterSettings,			{ "EffectEmitterSettings",				u8"\u0100" }},
	{ RESOURCE_TYPE_FXForceSettings,				{ "FXForceSettings",					u8"\u0100" }},
	{ RESOURCE_TYPE_FXSettings,						{ "FXSettings",							u8"\u0100" }},
	{ RESOURCE_TYPE_FarGroundLayout,				{ "FarGroundLayout",					u8"\u0100" }},
	{ RESOURCE_TYPE_FlareSettings,					{ "FlareSettings",						u8"\u0100" }},
	{ RESOURCE_TYPE_Font,							{ "Font",								u8"\u0100" }},
	{ RESOURCE_TYPE_GeoSettings,					{ "GeoSettings",						u8"\u0100" }},
	{ RESOURCE_TYPE_ImposterGroup,					{ "ImposterGroup",						u8"\u0100" }},
	{ RESOURCE_TYPE_LightGroup,						{ "LightGroup",							u8"\u0100" }},
	{ RESOURCE_TYPE_LightningSettings,				{ "LightningSettings",					u8"\u0100" }},
	{ RESOURCE_TYPE_Locators,						{ "Locators",							u8"\u0100" }},
	{ RESOURCE_TYPE_Material,						{ "Material",							u8"\u0101" }},
	{ RESOURCE_TYPE_MaterialTable,					{ "MaterialTable",						u8"\u0100" }},
	{ RESOURCE_TYPE_ModelData,						{ "ModelData",							u8"\u0102" }},
	{ RESOURCE_TYPE_MorphTargets,					{ "MorphTargets",						u8"\u0100" }},
	{ RESOURCE_TYPE_MovieResourceData,				{ "MovieResourceData",					u8"\u0100" }},
	{ RESOURCE_TYPE_NISSpatialData,					{ "NISSpatialData",						u8"\u0100" }},
	{ RESOURCE_TYPE_NavMeshData,					{ "NavMeshData",						u8"\u0100" }},
	{ RESOURCE_TYPE_ParkourContainer,				{ "ParkourContainer",					u8"\u0100" }},
	{ RESOURCE_TYPE_ParkourContainerBundle,			{ "ParkourContainerBundle",				u8"\u0100" }},
	{ RESOURCE_TYPE_ParkourInstance,				{ "ParkourInstance",					u8"\u0100" }},
	{ RESOURCE_TYPE_ParticleEmitterSettings,		{ "ParticleEmitterSettings",			u8"\u0100" }},
	{ RESOURCE_TYPE_PropertySet,					{ "PropertySet",						u8"\u0100" }},
	{ RESOURCE_TYPE_RasterState,					{ "RasterState",						u8"\u0100" }},
	{ RESOURCE_TYPE_ReflectResource,				{ "ReflectResource",					u8"\u0100" }},
	{ RESOURCE_TYPE_ReflectionGroup,				{ "ReflectionGroup",					u8"\u0100" }},
	{ RESOURCE_TYPE_Rig,							{ "Rig",								u8"\u0100" }},
	{ RESOURCE_TYPE_RigInfoResource,				{ "RigInfoResource",					u8"\u0100" }},
	{ RESOURCE_TYPE_RigInstance,					{ "RigInstance",						u8"\u0100" }},
	{ RESOURCE_TYPE_RoadNetwork,					{ "RoadNetwork",						u8"\u0100" }},
	{ RESOURCE_TYPE_SceneLayer,						{ "SceneLayer",							u8"\u0100" }},
	{ RESOURCE_TYPE_SceneryGroup,					{ "SceneryGroup",						u8"\u0100" }},
	{ RESOURCE_TYPE_ScreenParticleEmitterSettings,	{ "ScreenParticleEmitterSettings",		u8"\u0100" }},
	{ RESOURCE_TYPE_SectionEffects,					{ "SectionEffects",						u8"\u0100" }},
	{ RESOURCE_TYPE_SectionLayout,					{ "SectionLayout",						u8"\u0100" }},
	{ RESOURCE_TYPE_ShaderBinary,					{ "ShaderBinary",						u8"\u0100" }},
	{ RESOURCE_TYPE_ShaderTemplate,					{ "ShaderTemplate",						u8"\u0100" }},
	{ RESOURCE_TYPE_Sidewalk,						{ "Sidewalk",							u8"\u0100" }},
	{ RESOURCE_TYPE_StateBlock,						{ "StateBlock",							u8"\u0100" }},
	{ RESOURCE_TYPE_SymbolTableResource,			{ "SymbolTableResource",				u8"\u0100" }},
	{ RESOURCE_TYPE_TerrainData,					{ "TerrainData",						u8"\u0100" }},
	{ RESOURCE_TYPE_Texture,						{ "Texture",							u8"\u0105" }},
	{ RESOURCE_TYPE_TextureD3DResource,				{ "TextureD3DResource",					u8"\u0100" }},
	{ RESOURCE_TYPE_TrackStripSettings,				{ "TrackStripSettings",					u8"\u0100" }},
	{ RESOURCE_TYPE_TrueCrowdDataBase,				{ "TrueCrowdDataBase",					u8"\u0100" }},
	{ RESOURCE_TYPE_UELFragmentTable,				{ "UELFragmentTable",					u8"\u0100" }},
	{ RESOURCE_TYPE_UILocalization,					{ "UILocalization",						u8"\uF1AB" }},
	{ RESOURCE_TYPE_UIMinimapTile,					{ "UIMinimapTile",						u8"\u0100" }},
	{ RESOURCE_TYPE_UIScreen,						{ "UIScreen",							u8"\u0100" }},
	{ RESOURCE_TYPE_UniqueUIDTableResource,			{ "UniqueUIDTableResource",				u8"\u0100" }},
	{ RESOURCE_TYPE_VertexDecl,						{ "VertexDecl",							u8"\u0100" }},
	{ RESOURCE_TYPE_VolumetricEffectSettings,		{ "VolumetricEffectSettings",			u8"\u0100" }},
	{ RESOURCE_TYPE_WeightSetGroupResource,			{ "WeightSetGroupResource",				u8"\u0100" }},
	{ RESOURCE_TYPE_XMLFile,						{ "XMLFile",							u8"\u0100" }},
	{ RESOURCE_TYPE_ZoneLayout,						{ "ZoneLayout",							u8"\u0100" }}
};
static ResourceTypeInfo_t g_ResourceTypeUnknown = { "Unknown", u8"\u0100" };

namespace Resource
{
	__inline ResourceTypeInfo_t* GetTypeInfo(uint32_t p_TypeUID)
	{
		auto m_Find = g_ResourceTypeMap.find(p_TypeUID);
		if (m_Find != g_ResourceTypeMap.end())
			return &(*m_Find).second;

		return &g_ResourceTypeUnknown;
	}
}

class CResourceData : public UFG::ResourceData_t
{
public:

};