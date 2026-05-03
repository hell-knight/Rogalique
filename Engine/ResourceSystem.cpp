#include "pch.h"
#include "ResourceSystem.h"
#include "Logger.h"

namespace MyEngine
{
	ResourceSystem* ResourceSystem::Instance()
	{
		static ResourceSystem resourceSystem;
		return &resourceSystem;
	}

	void ResourceSystem::LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth)
	{
		LOG_INFO("Loading texture '" + name + "' from" + sourcePath);

		if (textures.find(name) != textures.end())
		{
			LOG_INFO("Texture '" + name + "' already loaded, skipping.");
			return;
		}

		sf::Texture* newTexture = new sf::Texture();
		if (newTexture->loadFromFile(sourcePath))
		{
			newTexture->setSmooth(isSmooth);
			textures.emplace(name, newTexture);
			LOG_INFO("Texture '" + name + "' loaded successfully.");
		}
		else
		{
			LOG_ERROR("Failed to load texture '" + name + "' from" + sourcePath);
			delete newTexture;
		}
	}

	const sf::Texture* ResourceSystem::GetTextureShared(const std::string& name) const
	{
		auto it = textures.find(name);
		if (it == textures.end())
		{
			LOG_ERROR("Texture not found: " + name);
			return nullptr;
		}
		return it->second;
	}

	sf::Texture* ResourceSystem::GetTextureCopy(const std::string& name) const
	{
		return new sf::Texture(*textures.find(name)->second);
	}

	void ResourceSystem::DeleteSharedTexture(const std::string& name)
	{
		auto texturePair = textures.find(name);

		sf::Texture* deletingTexure = texturePair->second;
		textures.erase(texturePair);
		delete deletingTexure;
	}

	void ResourceSystem::LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements, bool isSmooth)
	{
		LOG_INFO("Loading texture map '" + name + "' from" + sourcePath +
			" (" + std::to_string(totalElements) + " elements)");
		if (textureMaps.find(name) != textureMaps.end())
		{
			LOG_INFO("Texture map '" + name + "' already loaded, skipping.");
			return;
		}

		sf::Texture textureMap;
		if (!textureMap.loadFromFile(sourcePath))
		{
			LOG_ERROR("Failed to load texture map '" + name + "' from" + sourcePath);
		}

		auto textureMapElements = std::make_unique<std::vector<sf::Texture*>>();

		auto textureSize = textureMap.getSize();
		int loadedElements = 0;

		for (int y = 0; y <= textureSize.y - elementPixelSize.y; y += elementPixelSize.y)
		{
			if (loadedElements == totalElements)
			{
				break;
			}

			for (int x = 0; x <= textureSize.x - elementPixelSize.x; x += elementPixelSize.x)
			{
				if (loadedElements == totalElements)
				{
					break;
				}

				sf::Texture* newTextureMapElement = new sf::Texture();
				if (newTextureMapElement->loadFromFile(sourcePath, sf::IntRect(x, y, elementPixelSize.x, elementPixelSize.y)))
				{
					newTextureMapElement->setSmooth(isSmooth);
					textureMapElements->push_back(newTextureMapElement);
					loadedElements++;
				}
				else
				{
					LOG_WARN("Failed to extract element " + std::to_string(loadedElements) +
						" from texture map '" + name + "'. Skipping.");
					delete newTextureMapElement;
				}
			}
		}

		if (loadedElements == 0)
		{
			LOG_ERROR("No elements could be loaded for texture map '" + name + "'. Map will be empty.");
		}
		else
		{
			LOG_INFO("Texture map '" + name + "' loaded with " + std::to_string(loadedElements) +
				" elements (requested " + std::to_string(totalElements) + ").");
		}

		textureMaps.emplace(name, std::move(*textureMapElements));
	}

	const sf::Texture* ResourceSystem::GetTextureMapElementShared(const std::string& name, int elementIndex) const
	{
		auto textureMap = textureMaps.find(name);
		if (textureMap == textureMaps.end())
		{
			LOG_ERROR("Texture map not found: " + name);
			return nullptr;
		}

		const auto& textures = textureMap->second;
		if (elementIndex < 0 || elementIndex >= static_cast<int>(textures.size()))
		{
			LOG_ERROR("Texture map element index " + std::to_string(elementIndex) + " out of range for '" +
				name + "' (size: " + std::to_string(textures.size()) + ").");
			return nullptr;
		}

		return textures[elementIndex];
	}

	sf::Texture* ResourceSystem::GetTextureMapElementCopy(const std::string& name, int elementIndex) const
	{
		auto textureMap = textureMaps.find(name);
		auto textures = textureMap->second;
		return new sf::Texture(*textures[elementIndex]);
	}

	int ResourceSystem::GetTextureMapElementsCount(const std::string& name) const
	{
		auto textureMap = textureMaps.find(name);
		auto textures = textureMap->second;
		return textures.size();
	}

	void ResourceSystem::DeleteSharedTextureMap(const std::string& name)
	{
		auto textureMap = textureMaps.find(name);
		auto deletingTextures = textureMap->second;

		for (int i = 0; i < deletingTextures.size(); i++)
		{
			delete deletingTextures[i];
		}

		textureMaps.erase(textureMap);
	}

	void ResourceSystem::LoadSound(const std::string& name, std::string sourcePath)
	{
		LOG_INFO("Loading sound '" + name + "' from" + sourcePath);

		if (sounds.find(name) != sounds.end())
		{
			LOG_INFO("Sound '" + name + "' already loaded, skipping.");
			return;
		}

		sf::SoundBuffer* newSound = new sf::SoundBuffer();
		if (newSound->loadFromFile(sourcePath))
		{
			sounds.emplace(name, newSound);
			LOG_INFO("Sound '" + name + "' loaded successfully.");
		}
		else
		{
			LOG_ERROR("Failed to load sound '" + name + "' from" + sourcePath);
			delete newSound;
		}
	}

	const sf::SoundBuffer* ResourceSystem::GetSound(const std::string& name) const
	{
		auto it = sounds.find(name);
		if (it == sounds.end())
		{
			LOG_ERROR("Attempt to get sound '" + name + "' which is not loaded.");
			return nullptr;
		}
		return it->second;
	}

	void ResourceSystem::DeleteSound(const std::string& name)
	{
		auto soundPair = sounds.find(name);

		sf::SoundBuffer* deletingSound = soundPair->second;
		sounds.erase(soundPair);
		delete deletingSound;
	}

	void ResourceSystem::Clear()
	{
		DeleteAllTextures();
		DeleteAllTextureMaps();
		DeleteAllSounds();
	}

	void ResourceSystem::DeleteAllTextures()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& texturePair : textures)
		{
			keysToDelete.push_back(texturePair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSharedTexture(key);
		}
	}

	void ResourceSystem::DeleteAllTextureMaps()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& textureMapPair : textureMaps)
		{
			keysToDelete.push_back(textureMapPair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSharedTextureMap(key);
		}
	}

	void ResourceSystem::DeleteAllSounds()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& soundPair : sounds)
		{
			keysToDelete.push_back(soundPair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSound(key);
		}
	}
}