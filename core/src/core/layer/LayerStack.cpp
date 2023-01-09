#include "_Core.h"
#include "LayerStack.h"


namespace core
{
	LayerStack::LayerStack()
	{
		
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : layers)
		{
			delete layer;
		}
		layers.clear();
	}

	void LayerStack::addLayer(Layer* layer)
	{
		layers.emplace(layers.begin() + insertLayerIndex, layer);
		insertLayerIndex++;
	}

	void LayerStack::addOverlay(Layer* layer)
	{
		layers.emplace_back(layer);
		layer->setOverlayStatus(true);
	}

	void LayerStack::removeLayer(Layer* layer)
	{
		auto it = std::find(layers.begin(), layers.begin() + insertLayerIndex, layer);
		if (it != layers.begin() + insertLayerIndex)
		{
			layer->detach();
			layers.erase(it);
			insertLayerIndex--;
		}
	}

	void LayerStack::removeOverlay(Layer* layer)
	{
		auto it = std::find(layers.begin() + insertLayerIndex, layers.end(), layer);
		if (it != layers.end()) 
		{
			layer->detach();
			layers.erase(it);
		}
	}





}