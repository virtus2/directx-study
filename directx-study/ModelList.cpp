#include "ModelList.h"

ModelList::ModelList()
{
	modelInfoList = 0;
}

ModelList::ModelList(const ModelList&)
{
}

ModelList::~ModelList()
{
}

bool ModelList::Initialize(int numModels)
{
	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;

	// Store the number of models.
	modelCount = numModels;

	// Create a list array of the model information.
	modelInfoList = new ModelInfoType[modelCount];
	if(!modelInfoList)
	{
		return false;
	}

	// Seed the random generator with the current time.
	srand((unsigned int)time(NULL));

	// Go through all the models and randomly generate the model color and position.
	for(int i=0; i<modelCount; i++)
	{
		// Generate a random color for the model.
		red = (float)rand() / RAND_MAX;
		green = (float)rand() / RAND_MAX;
		blue = (float)rand() / RAND_MAX;

		modelInfoList[i].color = XMFLOAT4(red, green, blue, 1.0f);

		// Generate a random position in front of the viewer for the mode.
		modelInfoList[i].positionX = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		modelInfoList[i].positionY = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		modelInfoList[i].positionZ = ((((float)rand() - (float)rand()) / RAND_MAX) * 10.0f) + 5.0f;
	}
	return true;
}

void ModelList::Shutdown()
{
	if(modelInfoList)
	{
		delete[] modelInfoList;
		modelInfoList = 0;
	}
}

int ModelList::GetModelCount()
{
	return modelCount;
}

void ModelList::GetData(int index, float& positionX, float& positionY, float& positionZ, XMFLOAT4& color)
{
	positionX = modelInfoList[index].positionX;
	positionY = modelInfoList[index].positionY;
	positionZ = modelInfoList[index].positionZ;

	color = modelInfoList[index].color;
}
