#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include "mesh.hpp"

struct Helicopter {
	Mesh body = Mesh("<missing>");
	Mesh mainRotor = Mesh("<missing>");
	Mesh tailRotor = Mesh("<missing>");
	Mesh door = Mesh("<missing>");
};

struct CursedMesh 
{
	Mesh eyes = Mesh("<missing>");
	Mesh body = Mesh("<missing>");
};

std::vector<VectorMesh> loadWavefront(std::string const srcFile, bool quiet = false);

std::vector<VectorMesh> loadWavefront(std::string const srcFile, bool quiet);

CursedMesh loadCursedModel(std::string const srcFile);
Helicopter loadHelicopterModel(std::string const srcFile);
Mesh loadTerrainMesh(std::string const srcFile);