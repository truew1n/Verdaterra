#include "Texture.h"
#include <iostream>

uint32_t CTexture::MMaxUnit = 0;

CTexture::CTexture(const char *ITexturePath)
{
	MType = ETextureType::None;
	MUnit = 0;
	int32_t TextureWidth, TextureHeight;
	stbi_set_flip_vertically_on_load(true);
	uint8_t *TextureData = stbi_load(ITexturePath, &TextureWidth, &TextureHeight, &MChannels, 0);

	if (!TextureData) {
		std::cerr << "CTexture :: Failed to load texture: " << ITexturePath << "\n";
		return;
	}

	MUnit = ++MMaxUnit;

	glGenTextures(1, &Id);
	glActiveTexture(GL_TEXTURE0 + MUnit);
	glBindTexture(GL_TEXTURE_2D, Id);

	SetTextureParameter(ETextureParameter::MinFilter, ETextureParameterValue::LinearMipMapLinear);
	SetTextureParameter(ETextureParameter::MagFilter, ETextureParameterValue::Linear);

	SetTextureParameter(ETextureParameter::WrapU, ETextureParameterValue::Repeat);
	SetTextureParameter(ETextureParameter::WrapV, ETextureParameterValue::Repeat);

	switch (MChannels) {
		case 4: {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureWidth, TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
			break;
		}
		case 3: {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureWidth, TextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
			break;
		}
		case 1: {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, TextureWidth, TextureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, TextureData);
			break;
		}
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(TextureData);
}

void CTexture::SetTextureParameter(ETextureParameter Type, ETextureParameterValue Value)
{
	glTexParameteri(GL_TEXTURE_2D, (GLenum)Type, (GLint)Value);
}

void CTexture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + MUnit);
	glBindTexture(GL_TEXTURE_2D, Id);
}

void CTexture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

CTexture::~CTexture()
{
	glDeleteTextures(1, &Id);
}