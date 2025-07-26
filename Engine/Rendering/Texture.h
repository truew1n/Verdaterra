#ifndef EN_TEXTURE_H
#define EN_TEXTURE_H

#include <glad/glad.h>
#include "stb_image.h"

#include "RenderObject.h"
#include "Handle.h"
#include "Bind.h"
#include "DeviceBuffer.h"

enum class ETextureFormat : uint32_t {
	Red = GL_RED,
	RedGreen = GL_RG,
	RedGreenBlue = GL_RGB,
	BlueGreenRed = GL_BGR,
	RedGreenBlueAlpha = GL_RGBA,
	BlueGreenRedAlpha = GL_BGRA,
	Depth = GL_DEPTH_COMPONENT,
	Stencil = GL_STENCIL_INDEX
};


enum class ETextureType : uint32_t {
	Texture1D = GL_TEXTURE_1D,
	Texture2D = GL_TEXTURE_2D,
	Texture3D = GL_TEXTURE_3D,
	Texture1DArray = GL_TEXTURE_1D_ARRAY,
	Texture2DArray = GL_TEXTURE_2D_ARRAY,
	TextureRectangle = GL_TEXTURE_RECTANGLE,
	TextureCubeMap = GL_TEXTURE_CUBE_MAP,
	TextureCubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
	TextureBuffer = GL_TEXTURE_BUFFER,
	Texture2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
	Texture2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
};


enum class ETextureComponentType : uint32_t {
	UInt8 = GL_UNSIGNED_BYTE,
	Int8 = GL_BYTE,
	UInt16 = GL_UNSIGNED_SHORT,
	Int16 = GL_SHORT,
	UInt32 = GL_UNSIGNED_INT,
	Int32 = GL_INT,
	Float32 = GL_FLOAT,

	UInt8_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
	UInt8_2_3_3_Rev = GL_UNSIGNED_BYTE_2_3_3_REV,

	UInt16_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
	UInt16_5_6_5_Rev = GL_UNSIGNED_SHORT_5_6_5_REV,

	UInt16_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
	UInt16_4_4_4_4_Rev = GL_UNSIGNED_SHORT_4_4_4_4_REV,

	UInt16_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
	UInt16_1_5_5_5_Rev = GL_UNSIGNED_SHORT_1_5_5_5_REV,

	UInt32_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
	UInt32_8_8_8_8_Rev = GL_UNSIGNED_INT_8_8_8_8_REV,

	UInt32_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
	UInt32_2_10_10_10_Rev = GL_UNSIGNED_INT_2_10_10_10_REV
};


enum class ETextureParameter : uint32_t {
	MinFilter = GL_TEXTURE_MIN_FILTER,
	MagFilter = GL_TEXTURE_MAG_FILTER,
	WrapU = GL_TEXTURE_WRAP_S,
	WrapV = GL_TEXTURE_WRAP_T
};


enum class ETextureMinFilter {
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR,
	NearestMipMapNearest = GL_NEAREST_MIPMAP_NEAREST,
	LinearMipMapNearest = GL_LINEAR_MIPMAP_NEAREST,
	NearestMipMapLinear = GL_NEAREST_MIPMAP_LINEAR,
	LinearMipMapLinear = GL_LINEAR_MIPMAP_LINEAR
};


enum class ETextureMagFilter {
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR
};


enum class ETextureWrap {
	ClampToEdge = GL_CLAMP_TO_EDGE,
	ClampToBorder = GL_CLAMP_TO_BORDER,
	MirroredRepeat = GL_MIRRORED_REPEAT,
	Repeat = GL_REPEAT,
	MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
};


enum class ETextureUsage : uint8_t {
	Diffuse = 1,
	Specular = 2,
	Ambient = 3,
	Emissive = 4,
	Height = 5,
	Normals = 6,
	Shininess = 7,
	Opacity = 8,
	Displacement = 9,
	Lightmap = 10,
	Reflection = 11,
	BaseColor = 12,
	NormalCamera = 13,
	EmissionColor = 14,
	Metalness = 15,
	DiffuseRoughness = 16,
	AmbientOcclusion = 17,
	Unknown = 18,
	Sheen = 19,
	ClearCoat = 20,
	Transmission = 21,
	MayaBase = 22,
	MayaSpecular = 23,
	MayaSpecularColor = 24,
	MayaSpecularRoughness = 25,
	None = 0
};


class CTexture : public CRenderObject, public CHandle, public IBind {
private:
	inline static uint32_t MMaxUnit = 0;

	ETextureFormat MFormat;
	ETextureUsage MUsage;
	uint32_t MUnit;

	int32_t MChannels;
private:
	uint32_t GetNextUnit();
public:
	virtual void Create() override;
	virtual void Create(ETextureType Type);
	virtual void Create(const char *Filepath);
	virtual void Create(const char *Filepath, ETextureType Type);

	virtual void Create(ETextureFormat Format);
	virtual void Create(ETextureType Type, ETextureFormat Format);
	virtual void Create(const char *Filepath, ETextureFormat Format);
	virtual void Create(const char *Filepath, ETextureType Type, ETextureFormat Format);

	virtual void Bind() override;

	void SetMinFilter(ETextureMinFilter Value);
	void SetMagFilter(ETextureMagFilter Value);
	void SetHorizontalWrap(ETextureWrap Value);
	void SetVerticalWrap(ETextureWrap Value);
	
	ETextureUsage GetUsage() const;
	ETextureFormat GetFormat() const;
	void SetUsage(ETextureUsage Usage);
	uint32_t GetUnit() const;
	uint32_t GetChannels() const;

	virtual void Unbind() override;
	virtual void Destroy() override;
};

#endif