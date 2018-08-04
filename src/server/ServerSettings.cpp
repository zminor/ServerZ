#include "ServerSettings.h"

namespace HttpServer
{
	ServerSettings::~ServerSettings()
	{
		this->clear();
	}	
		void ServerSettings::addDataVariant(DataVariant::Abstract *dataVariant)
		{
			this->variants.emplace(
								dataVariant->getName(),
								dataVariant
								);
		}
		
		void ServerSettings::clear()
		{
			if (this->variants.empty() == false)
			{
				for (auto &variant : this->variants)
				{
					delete variant.second;
				}
				
				this->variants.clear();
			}
			this->global.clear();
		}		
}
