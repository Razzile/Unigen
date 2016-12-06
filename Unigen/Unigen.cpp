// Unigen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include  "il2cpp.h"
#include <assert.h>
#include <vector>
#include "MemoryStream.h"

int main(int argc, char **argv) {
	MemoryStream *stream = new MemoryStream(argv[1]);
	Il2CppGlobalMetadataHeader metadata = stream->Read<Il2CppGlobalMetadataHeader>();
	assert(metadata.sanity == 0xFAB11BAF && metadata.version == 21);
	
	int image_count = metadata.imagesCount / sizeof(Il2CppImageDefinition);
	int type_count = metadata.typeDefinitionsCount / sizeof(Il2CppTypeDefinition);

	char *image_def_offset = (char *)stream->base() + metadata.imagesOffset;
	std::vector<Il2CppImageDefinition> image_definitions(image_count);
	image_definitions.assign(image_def_offset, image_def_offset + image_count);

	for (auto& def : image_definitions) {

	}
}

