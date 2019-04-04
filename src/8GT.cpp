#include "Encoders.hpp"
#include <string.h>

struct Module8GT : Module
{
	Module8GT() : Module(0, 8, 1, 0)
	{
		memset( m_bits, 0, sizeof m_bits );
	}
	void step() override;
	
	bool	m_bits[8];
};

void Module8GT::step()
{
	float out = 0.0f;
	
	for ( int i=0; i<8; ++i )
	{
		float f = inputs[i].value;
		if ( m_bits[i] )
		{
			if ( f < 0.5f )
				m_bits[i] = false;
		}
		else
		{
			if ( f >= 1.0f )
				m_bits[i] = true;
		}
		f = m_bits[i] ? (1<<i) : 0;
		out += f;
	}

	outputs[0].value = out;
}

struct Module8GTWidget : ModuleWidget
{
	Module8GTWidget(Module8GT *module) : ModuleWidget(module)
	{
		setPanel(SVG::load(assetPlugin(plugin, "res/8GT.svg")));

		for ( int i=0; i<8; ++i )
			addInput(Port::create<PJ301MPort>(Vec(17, 45+33*i), Port::INPUT, module, i));

		addOutput(Port::create<PJ301MPort>(Vec(17, 10*33), Port::OUTPUT, module, 0));
	}
};

// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *model8GT = Model::create<Module8GT, Module8GTWidget>( "Expert Sleepers", "ExpertSleepers-Encoders-8GT", "8GT Encoder", EXTERNAL_TAG );
