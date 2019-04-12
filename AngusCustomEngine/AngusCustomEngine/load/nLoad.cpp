#include "nLoad.h"
#include "sRigidBodyDef.h"
#include "sSoftBodyDef.h"
#include <iostream>
#include <fstream>
#include <../load/json/json.h>
#include <../load/sConfig.h>
#include <eShapeType.h>

namespace nLoad
{
	bool LoadConfig(const char* fileName, sConfig& configOut)
	{
		Json::Value jsonConfig;
		if (!ReadJsonFile(fileName, jsonConfig))
		{
			//do something about this!
			std::cout << "Error reading Json File" << std::endl;
			return false;
		}

		//do the library
		Json::Value jsonPhysicsLibrary = jsonConfig["PhysicsLibrary"];
		if (jsonPhysicsLibrary.isString())
		{
			configOut.PhysicsLibrary = jsonPhysicsLibrary.asString();
		}
		else
		{
			std::cout << "Missing Library from file - SuperDuperA.dll is being used by default" << std::endl;
			configOut.PhysicsLibrary = "SimplePhysics.dll";
		}


		//********************************
		//	SOFTBODY DEFS
		//********************************

		//SoftBodies
		Json::Value jsonSoftBodyDefs = jsonConfig["SoftBodyDefs"];
		if (!jsonSoftBodyDefs.isNull())
		{
			if (!jsonSoftBodyDefs.isArray())
			{
				// do something about this
				return false;
			}
			size_t numSoftDefs = jsonSoftBodyDefs.size();

			for (size_t c = 0; c < numSoftDefs; c++)
			{
				Json::Value jsonSoftBodyDef = jsonSoftBodyDefs[(int)c];
				if (jsonSoftBodyDef.isObject())
				{
					if (strcmp(jsonSoftBodyDef["Type"].asString().c_str(), "Cloth") == 0)
					{
						configOut.SoftObjTypes.push_back(jsonSoftBodyDef["Type"].asString());
						nPhysics::sSoftBodyDef def;
						if (jsonSoftBodyDef["Shape"].isString())
						{
							std::string shape = jsonSoftBodyDef["Shape"].asString();
							if (strcmp(shape.c_str(), "Cloth") == 0)
							{
								configOut.SoftShapeTypes.push_back("Cloth");
							}
						}
						Json::Value jsonSpringConstant = jsonSoftBodyDef["SpringConstant"];
						if (!jsonSpringConstant.isNumeric())
						{
							std::cout << "No valid SpringConstant for entity " << std::to_string(c) << std::endl;
							return false;
						}
						Json::Value jsonNumNodesAB = jsonSoftBodyDef["NumNodesAB"];
						if (!jsonNumNodesAB.isNumeric())
						{
							std::cout << "No valid NumNodesAB for entity " << std::to_string(c) << std::endl;
							return false;
						}
						Json::Value jsonNumNodesAC = jsonSoftBodyDef["NumNodesAC"];
						if (!jsonNumNodesAC.isNumeric())
						{
							std::cout << "No valid NumNodesAC for entity " << std::to_string(c) << std::endl;
							return false;
						}
						Json::Value jsonCornerA = jsonSoftBodyDef["CornerA"];
						if (!jsonCornerA.isArray())
						{
							std::cout << "jsonCornerA not a valid array for entity " << std::to_string(c) << std::endl;
							return false;
						}
						Json::Value jsonCornerB = jsonSoftBodyDef["CornerB"];
						if (!jsonCornerB.isArray())
						{
							std::cout << "jsonCornerB not a valid array for entity " << std::to_string(c) << std::endl;
							return false;
						}
						Json::Value jsonCornerC = jsonSoftBodyDef["CornerC"];
						if (!jsonCornerC.isArray())
						{
							std::cout << "jsonCornerC not a valid array for entity " << std::to_string(c) << std::endl;
							return false;
						}

						def.SpringConstant = jsonSpringConstant.asFloat();
						glm::vec3 cornerA;
						glm::vec3 cornerB;
						glm::vec3 cornerC;
						cornerA.x = jsonCornerA[0].asFloat();
						cornerA.y = jsonCornerA[1].asFloat();
						cornerA.z = jsonCornerA[2].asFloat();
						cornerB.x = jsonCornerB[0].asFloat();
						cornerB.y = jsonCornerB[1].asFloat();
						cornerB.z = jsonCornerB[2].asFloat();
						cornerC.x = jsonCornerC[0].asFloat();
						cornerC.y = jsonCornerC[1].asFloat();
						cornerC.z = jsonCornerC[2].asFloat();
						size_t numNodesAB = jsonNumNodesAB.asUInt();
						size_t numNodesAC = jsonNumNodesAC.asUInt();

						//glm::vec3 position;
						glm::vec3 sepAtoB((cornerB - cornerA) / (float)(numNodesAB - 1)); //TODO: /0 error
						glm::vec3 sepAtoC((cornerC - cornerA) / (float)(numNodesAC - 1));

						def.numAB = numNodesAB;
						def.numAC = numNodesAC;

						for (size_t idxB = 0; idxB < numNodesAC; idxB++)
						{
							for (size_t idxA = 0; idxA < numNodesAB; idxA++)
							{
								def.Nodes.push_back(nPhysics::sSoftBodyNodeDef(cornerA + sepAtoB * float(idxA) + sepAtoC * float(idxB), 1.0f));
							}
						}

						for (size_t idxA = 0; idxA < numNodesAB; idxA++)
						{
							def.Nodes[idxA].Mass = 0.0f;
						}

						//TODO: springs
						for (size_t idxB = 0; idxB < numNodesAC - 1; idxB++) //vertical
						{
							for (size_t idxA = 0; idxA < numNodesAB - 1; idxA++) //horizontal
							{
								size_t index = idxA + idxB * numNodesAB;
								def.Springs.push_back(std::make_pair(index, index + 1));
								def.Springs.push_back(std::make_pair(index, index + numNodesAB));
							}
						}

						size_t numNodes = numNodesAB * numNodesAC;

						for (size_t idxA = 0; idxA < numNodesAB - 1; idxA++)
						{
							def.Springs.push_back(std::make_pair(numNodes - idxA - 1, numNodes - idxA - 2)); //last row horizontally
						}
						for (size_t idxB = 1; idxB < numNodesAC; idxB++)
						{
							size_t index = (numNodesAB * idxB) - 1;
							def.Springs.push_back(std::make_pair(index, index + numNodesAB));
						}

						configOut.SoftBodyDefs.push_back(def);
					}
				}
			}
		}

		//********************************
		//	RIGIDBODY DEFS
		//********************************

		//do the defs
		Json::Value jsonRigidBodyDefs = jsonConfig["RigidBodyDefs"];
		if (!jsonRigidBodyDefs.isArray())
		{
			// do something about this
			return false;
		}
		std::vector<Json::Value> jsonVector;

		//This will loop through an array of arrays
		for (Json::Value::ArrayIndex i = 0; i != jsonRigidBodyDefs.size(); i++)
		{
			if (jsonRigidBodyDefs[i].isMember("Spheres"))
				jsonVector.push_back(jsonRigidBodyDefs[i]["Spheres"]);
			else if (jsonRigidBodyDefs[i].isMember("Planes"))
				jsonVector.push_back(jsonRigidBodyDefs[i]["Planes"]);
			else if (jsonRigidBodyDefs[i].isMember("Boxes"))
				jsonVector.push_back(jsonRigidBodyDefs[i]["Boxes"]);
			else if (jsonRigidBodyDefs[i].isMember("Cylinders"))
				jsonVector.push_back(jsonRigidBodyDefs[i]["Cylinders"]);
			else if (jsonRigidBodyDefs[i].isMember("Cones"))
				jsonVector.push_back(jsonRigidBodyDefs[i]["Cones"]);
		}
		size_t numRigidDefs = jsonVector.size();
		for (size_t a = 0; a < numRigidDefs; a++)
		{
			Json::Value jsonBodyDefs = jsonVector.at(a);

			if (!jsonBodyDefs.isArray())
			{
				return false;
			}
			size_t numDefs = jsonBodyDefs.size();
			for (size_t c = 0; c < numDefs; c++)
			{
				Json::Value jsonBodyDef = jsonBodyDefs[(int)c];

				if (jsonBodyDef.isObject())
				{
					if (jsonBodyDef["Type"].isString())
					{
						if (strcmp(jsonBodyDef["Type"].asString().c_str(), "Simple") == 0)
						{
							configOut.RigidObjTypes.push_back(jsonBodyDef["Type"].asString().c_str());
							if (jsonBodyDef["Shape"].isString())
							{
								std::string shape = jsonBodyDef["Shape"].asString();
								if (strcmp(shape.c_str(), "Sphere") == 0)
								{
									configOut.RigidShapeTypes.push_back("Sphere");
								}
								else if (strcmp(shape.c_str(), "Plane0") == 0)
								{
									configOut.RigidShapeTypes.push_back("Plane0");
								}
								else if (strcmp(shape.c_str(), "Plane1") == 0)
								{
									configOut.RigidShapeTypes.push_back("Plane1");
								}
								else if (strcmp(shape.c_str(), "Plane2") == 0)
								{
									configOut.RigidShapeTypes.push_back("Plane2");
								}
								else if (strcmp(shape.c_str(), "Plane3") == 0)
								{
									configOut.RigidShapeTypes.push_back("Plane3");
								}
								else if (strcmp(shape.c_str(), "Plane4") == 0)
								{
									configOut.RigidShapeTypes.push_back("Plane4");
								}
								else if (strcmp(shape.c_str(), "Box") == 0)
								{
									configOut.RigidShapeTypes.push_back("Box");
								}
								else if (strcmp(shape.c_str(), "Cylinder") == 0)
								{
									configOut.RigidShapeTypes.push_back("Cylinder");
								}
								else if (strcmp(shape.c_str(), "Cone") == 0)
								{
									configOut.RigidShapeTypes.push_back("Cone");
								}
								else
								{
									std::cout << "Error: unrecognized shape" << std::endl;
									return false;
								}
							}
							//Mass(0.0f)
							//, Position(0.0f, 0.0f, 0.0f)
							//	, Velocity(0.0f, 0.0f, 0.0f)
							//	, Orientation(0.0f, 0.0f, 0.0f)
							nPhysics::sRigidBodyDef def;

							if (jsonBodyDef["Mass"].isNumeric())
							{
								def.Mass = jsonBodyDef["Mass"].asFloat();
							}
							else
							{
								std::cout << "No valid number for Mass - incorrect format" << std::endl;
								return false;
							}
							if (jsonBodyDef["Position.x"].isNumeric())
							{
								def.Position.x = jsonBodyDef["Position.x"].asFloat();
							}
							else
							{
								std::cout << "No valid number for Position.x - incorrect format" << std::endl;
								return false;
							}
							if (jsonBodyDef["Position.y"].isNumeric())
							{
								def.Position.y = jsonBodyDef["Position.y"].asFloat();
							}
							else
							{
								std::cout << "No valid number for Position.y - incorrect format" << std::endl;
								return false;
							}
							if (jsonBodyDef["Position.z"].isNumeric())
							{
								def.Position.z = jsonBodyDef["Position.z"].asFloat();
							}
							else
							{
								std::cout << "No valid number for Position.z - incorrect format" << std::endl;
								return false;
							}
							if (jsonBodyDef["Velocity.x"].isNumeric())
							{
								def.Velocity.x = jsonBodyDef["Velocity.x"].asFloat();
							}
							else
							{
								std::cout << "No valid number for Velocity.x - incorrect format" << std::endl;
								return false;
							}
							if (jsonBodyDef["Velocity.y"].isNumeric())
							{
								def.Velocity.y = jsonBodyDef["Velocity.y"].asFloat();
							}
							else
							{
								std::cout << "No valid number for Velocity.y - incorrect format" << std::endl;
								return false;
							}
							if (jsonBodyDef["Velocity.z"].isNumeric())
							{
								def.Velocity.z = jsonBodyDef["Velocity.z"].asFloat();
							}
							else
							{
								std::cout << "No valid number for Velocity.z - incorrect format" << std::endl;
								return false;
							}
							if (jsonBodyDef["Orientation.x"].isNumeric())
							{
								def.Orientation.x = jsonBodyDef["Orientation.x"].asFloat();
							}
							else
							{
								std::cout << "No valid number for Orientation.x - incorrect format" << std::endl;
								return false;
							}
							if (jsonBodyDef["Orientation.y"].isNumeric())
							{
								def.Orientation.y = jsonBodyDef["Orientation.y"].asFloat();
							}
							else
							{
								std::cout << "No valid number for Orientation.y - incorrect format" << std::endl;
								return false;
							}
							if (jsonBodyDef["Orientation.z"].isNumeric())
							{
								def.Orientation.z = jsonBodyDef["Orientation.z"].asFloat();
							}
							else
							{
								std::cout << "No valid number for Orientation.z - incorrect format" << std::endl;
								return false;
							}
							configOut.RigidBodyDefs.push_back(def);
						}
					}
				}
				else
				{
					std::cout << "Uhoh, not an object" << std::endl;
					return false;
				}
			}
		}
		return true;
	}

	bool ReadJsonFile(const char* fileName, Json::Value& jsonOut)
	{
		if (!FileExists(fileName))
		{
			//maybe log an error or something
			return false;
		}
		std::ifstream jsonFile(fileName, std::ifstream::binary);
		Json::CharReaderBuilder charReaderBuilder;
		std::string errs;
		bool success = Json::parseFromStream(charReaderBuilder, jsonFile, &jsonOut, &errs);
		return success;
	}
}