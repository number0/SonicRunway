//
//  BeatBouncePattern.cpp
//  SonicRunway
//
//  Created by Michael Frederickson on 7/6/16.
//
//

#include "BeatBouncePattern.hpp"
#include "Audio.hpp"
#include "BeatHistory.hpp"

SrBeatBouncePattern::SrBeatBouncePattern(const std::string & name,
                             SrModel * model, SrAudio * audio,
                             SrGlobalParameters * globalParameters) :
SrScrollingPattern(name, model, audio, globalParameters),
_hueParam(0.5),
_decayTimeParam(0.25),
_spinSpeedParam(0.25),
_spinOffset(0.2)
{
    _hueParam.setName("Hue");
    _hueParam.setMin(0.0);
    _hueParam.setMax(1.0);
    _AddUIParameter(_hueParam);
    
    _decayTimeParam.setName("DecayTime");
    _decayTimeParam.setMin(0.0);
    _decayTimeParam.setMax(1.0);
    _AddUIParameter(_decayTimeParam);
    
    _spinSpeedParam.setName("SpinSpeed");
    _spinSpeedParam.setMin(-3.0);
    _spinSpeedParam.setMax(3.0);
    _AddUIParameter(_spinSpeedParam);
 
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(1.0,0,0),
                                                   ofFloatColor(0.0, 1.0, 0.0)} );
    
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(1.0,1.0,0),
                                                   ofFloatColor(0.0, 0.0, 1.0)} );
     
    
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.980392156863,0.411764705882,0.0),
        ofFloatColor(0.654901960784,0.858823529412,0.847058823529),
        ofFloatColor(0.952941176471,0.525490196078,0.188235294118),
        ofFloatColor(0.878431372549,0.894117647059,0.8),
        ofFloatColor(0.411764705882,0.823529411765,0.905882352941)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.996078431373,0.262745098039,0.396078431373),
        ofFloatColor(0.988235294118,0.61568627451,0.603921568627),
        ofFloatColor(0.976470588235,0.803921568627,0.678431372549),
        ofFloatColor(0.513725490196,0.686274509804,0.607843137255),
        ofFloatColor(0.78431372549,0.78431372549,0.662745098039)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(1.0,0.419607843137,0.419607843137),
        ofFloatColor(0.76862745098,0.301960784314,0.345098039216),
        ofFloatColor(0.305882352941,0.803921568627,0.76862745098),
        ofFloatColor(0.780392156863,0.956862745098,0.392156862745),
        ofFloatColor(0.333333333333,0.38431372549,0.439215686275)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(1.0,0.623529411765,0.501960784314),
        ofFloatColor(0.960784313725,0.411764705882,0.56862745098),
        ofFloatColor(1.0,0.76862745098,0.549019607843),
        ofFloatColor(0.937254901961,0.980392156863,0.705882352941),
        ofFloatColor(0.819607843137,0.949019607843,0.647058823529)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.929411764706,0.788235294118,0.317647058824),
        ofFloatColor(0.8,0.2,0.247058823529),
        ofFloatColor(0.921568627451,0.407843137255,0.254901960784),
        ofFloatColor(0.41568627451,0.290196078431,0.235294117647),
        ofFloatColor(0.0,0.627450980392,0.690196078431)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.0,0.705882352941,0.8),
        ofFloatColor(0.0,0.372549019608,0.419607843137),
        ofFloatColor(0.0,0.874509803922,0.988235294118),
        ofFloatColor(0.0,0.549019607843,0.619607843137)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.988235294118,0.56862745098,0.227450980392),
        ofFloatColor(0.929411764706,0.898039215686,0.454901960784),
        ofFloatColor(1.0,0.305882352941,0.313725490196),
        ofFloatColor(0.976470588235,0.83137254902,0.137254901961)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.976470588235,0.949019607843,0.905882352941),
        ofFloatColor(0.682352941176,0.886274509804,0.223529411765),
        ofFloatColor(0.560784313725,0.745098039216,0.0),
        ofFloatColor(0.250980392157,0.752941176471,0.796078431373),
        ofFloatColor(0.0,0.658823529412,0.776470588235)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.980392156863,0.164705882353,0.0),
        ofFloatColor(0.949019607843,0.839215686275,0.580392156863),
        ofFloatColor(0.941176470588,0.847058823529,0.658823529412),
        ofFloatColor(0.525490196078,0.721568627451,0.694117647059),
        ofFloatColor(0.239215686275,0.109803921569,0.0)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(1.0,0.96862745098,0.741176470588),
        ofFloatColor(0.941176470588,0.254901960784,0.333333333333),
        ofFloatColor(0.58431372549,0.811764705882,0.717647058824),
        ofFloatColor(1.0,0.509803921569,0.227450980392),
        ofFloatColor(0.949019607843,0.949019607843,0.435294117647)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.764705882353,1.0,0.407843137255),
        ofFloatColor(0.956862745098,0.988235294118,0.909803921569),
        ofFloatColor(0.529411764706,0.839215686275,0.607843137255),
        ofFloatColor(0.494117647059,0.81568627451,0.839215686275),
        ofFloatColor(0.305882352941,0.588235294118,0.537254901961)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.949019607843,0.113725490196,0.254901960784),
        ofFloatColor(0.921568627451,0.921568627451,0.737254901961),
        ofFloatColor(0.737254901961,0.890196078431,0.772549019608),
        ofFloatColor(0.509803921569,0.701960784314,0.682352941176)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.764705882353,1.0,0.407843137255),
        ofFloatColor(0.498039215686,1.0,0.141176470588),
        ofFloatColor(0.0862745098039,0.576470588235,0.647058823529),
        ofFloatColor(0.0,0.803921568627,0.674509803922),
        ofFloatColor(0.0078431372549,0.666666666667,0.690196078431)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.988235294118,0.850980392157,0.898039215686),
        ofFloatColor(0.98431372549,0.772549019608,0.847058823529),
        ofFloatColor(0.945098039216,0.223529411765,0.427450980392),
        ofFloatColor(1.0,0.917647058824,0.949019607843)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.227450980392,0.537254901961,0.788235294118),
        ofFloatColor(0.949019607843,0.423529411765,0.309803921569),
        ofFloatColor(0.913725490196,0.949019607843,0.976470588235),
        ofFloatColor(0.611764705882,0.76862745098,0.894117647059),
        ofFloatColor(0.105882352941,0.196078431373,0.372549019608)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.98431372549,0.419607843137,0.254901960784),
        ofFloatColor(0.964705882353,0.847058823529,0.419607843137),
        ofFloatColor(0.945098039216,0.0470588235294,0.286274509804),
        ofFloatColor(0.654901960784,0.0078431372549,0.403921568627)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.533333333333,0.756862745098,0.0),
        ofFloatColor(1.0,0.541176470588,0.0),
        ofFloatColor(0.0,0.756862745098,0.462745098039),
        ofFloatColor(1.0,0.0,0.235294117647),
        ofFloatColor(0.980392156863,0.745098039216,0.156862745098)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.666666666667,1.0,0.0),
        ofFloatColor(1.0,0.666666666667,0.0),
        ofFloatColor(0.666666666667,0.0,1.0),
        ofFloatColor(1.0,0.0,0.666666666667),
        ofFloatColor(0.0,0.666666666667,1.0)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.0627450980392,0.498039215686,0.788235294118),
        ofFloatColor(0.0470588235294,0.0588235294118,0.4),
        ofFloatColor(0.0274509803922,0.0352941176471,0.239215686275),
        ofFloatColor(0.0549019607843,0.305882352941,0.678431372549),
        ofFloatColor(0.043137254902,0.0627450980392,0.549019607843)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.894117647059,0.960784313725,0.694117647059),
        ofFloatColor(0.654901960784,0.858823529412,0.670588235294),
        ofFloatColor(0.317647058824,0.16862745098,0.321568627451),
        ofFloatColor(0.482352941176,0.690196078431,0.658823529412),
        ofFloatColor(0.388235294118,0.321568627451,0.454901960784)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.952941176471,0.278431372549,0.223529411765),
        ofFloatColor(0.850980392157,0.0588235294118,0.352941176471),
        ofFloatColor(0.6,0.0901960784314,0.4),
        ofFloatColor(0.2,0.0745098039216,0.152941176471),
        ofFloatColor(1.0,0.43137254902,0.152941176471)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.952941176471,1.0,0.921568627451),
        ofFloatColor(0.945098039216,0.223529411765,0.427450980392),
        ofFloatColor(0.674509803922,0.788235294118,0.372549019608),
        ofFloatColor(0.560784313725,0.6,0.141176470588),
        ofFloatColor(0.992156862745,0.376470588235,0.505882352941)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.478431372549,0.756862745098,0.76862745098),
        ofFloatColor(0.439215686275,0.686274509804,0.76862745098),
        ofFloatColor(0.890196078431,0.909803921569,0.811764705882),
        ofFloatColor(0.678431372549,0.847058823529,0.780392156863),
        ofFloatColor(0.549019607843,0.8,0.745098039216)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(1.0,0.741176470588,0.847058823529),
        ofFloatColor(0.576470588235,0.874509803922,0.721568627451),
        ofFloatColor(1.0,0.78431372549,0.729411764706),
        ofFloatColor(0.890196078431,0.666666666667,0.839215686275),
        ofFloatColor(0.709803921569,0.847058823529,0.921568627451)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.211764705882,0.0274509803922,0.270588235294),
        ofFloatColor(0.105882352941,0.529411764706,0.596078431373),
        ofFloatColor(0.937254901961,0.917647058824,0.772549019608),
        ofFloatColor(0.905882352941,0.847058823529,0.294117647059),
        ofFloatColor(0.839215686275,0.109803921569,0.349019607843)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.854901960784,0.866666666667,0.98431372549),
        ofFloatColor(0.854901960784,0.929411764706,0.98431372549),
        ofFloatColor(0.854901960784,0.98431372549,0.972549019608),
        ofFloatColor(0.972549019608,0.854901960784,0.98431372549),
        ofFloatColor(0.909803921569,0.854901960784,0.98431372549)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.996078431373,0.262745098039,0.396078431373),
        ofFloatColor(0.988235294118,0.61568627451,0.603921568627),
        ofFloatColor(0.976470588235,0.803921568627,0.678431372549),
        ofFloatColor(0.513725490196,0.686274509804,0.607843137255),
        ofFloatColor(0.78431372549,0.78431372549,0.662745098039)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(1.0,0.772549019608,0.172549019608),
        ofFloatColor(0.905882352941,0.929411764706,0.917647058824),
        ofFloatColor(0.0117647058824,0.0509803921569,0.309803921569),
        ofFloatColor(0.98431372549,0.0470588235294,0.0235294117647),
        ofFloatColor(0.807843137255,0.925490196078,0.937254901961)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.549019607843,0.819607843137,0.61568627451),
        ofFloatColor(1.0,0.321568627451,0.329411764706),
        ofFloatColor(0.988235294118,0.713725490196,0.325490196078),
        ofFloatColor(0.807843137255,0.909803921569,0.474509803922),
        ofFloatColor(0.360784313725,0.674509803922,0.76862745098)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.996078431373,0.976470588235,0.941176470588),
        ofFloatColor(0.462745098039,0.827450980392,0.870588235294),
        ofFloatColor(0.690196078431,0.972549019608,1.0),
        ofFloatColor(0.682352941176,0.909803921569,0.98431372549),
        ofFloatColor(0.0,0.737254901961,0.819607843137)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.956862745098,0.952941176471,0.156862745098),
        ofFloatColor(0.874509803922,0.0823529411765,0.101960784314),
        ofFloatColor(0.0,0.796078431373,0.905882352941),
        ofFloatColor(0.992156862745,0.525490196078,0.0117647058824),
        ofFloatColor(0.0,0.854901960784,0.235294117647)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(1.0,0.662745098039,0.152941176471),
        ofFloatColor(0.988235294118,0.419607843137,0.0392156862745),
        ofFloatColor(0.972549019608,0.529411764706,0.180392156863),
        ofFloatColor(0.992156862745,0.792156862745,0.286274509804),
        ofFloatColor(0.988235294118,0.345098039216,0.0470588235294)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.780392156863,0.929411764706,0.909803921569),
        ofFloatColor(0.270588235294,0.709803921569,0.76862745098),
        ofFloatColor(0.0862745098039,0.576470588235,0.647058823529),
        ofFloatColor(0.627450980392,0.870588235294,0.839215686275),
        ofFloatColor(0.494117647059,0.807843137255,0.792156862745)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(0.305882352941,0.701960784314,0.870588235294),
        ofFloatColor(0.988235294118,0.941176470588,0.623529411765),
        ofFloatColor(0.949019607843,0.486274509804,0.486274509804),
        ofFloatColor(0.870588235294,0.321568627451,0.549019607843),
        ofFloatColor(0.552941176471,0.878431372549,0.650980392157)} );
    _palettes.push_back( std::vector<ofFloatColor>{ofFloatColor(1.0,0.129411764706,0.129411764706),
        ofFloatColor(0.639215686275,0.890196078431,0.694117647059),
        ofFloatColor(0.317647058824,0.741176470588,0.611764705882),
        ofFloatColor(1.0,0.909803921569,0.701960784314),
        ofFloatColor(0.141176470588,0.258823529412,0.258823529412)} );

}

SrBeatBouncePattern::~SrBeatBouncePattern()
{
    
}

void
SrBeatBouncePattern::_Update()
{
    SrScrollingPattern::_Update();
    
    _spinOffset += (float) _spinSpeedParam;
}

std::vector<ofFloatColor>
SrBeatBouncePattern::_RandomPalette() const
{
    int globalBeat = GetAudio()->GetBeatHistory().GetGlobalBeat()[0];
    int globalMeasure = (int)globalBeat / 8;
    
    srand(globalMeasure);
    int index = rand() % _palettes.size();

    // Clamp to buffer size just to be safe
    index = std::min(index, (int) _palettes.size() - 1);
    return _palettes[index];
}

void
SrBeatBouncePattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    float hue = (float) _hueParam;
    
    float timeSinceBeat =
    GetAudio()->GetBeatHistory().GetSecondsSinceLastEvent()[0];
    
    float bpm =
    GetAudio()->GetBeatHistory().GetBpm()[0];
    
    /**************/
    
    float onsetDecayTime = (float) _decayTimeParam;
    
    float onsetAmount = 1.0 - (timeSinceBeat / onsetDecayTime);
    
    if (onsetAmount < 0.0) {
        return;
    }
    
    float onsetMult = onsetAmount;
    
    // Force full onsetAmount if we're less than one full gate behind.
    if (timeSinceBeat < GetModel()->ComputeDelayPerGate()) {
        onsetMult = 1.0;
    }
    
    onsetMult *= onsetMult;
    
    ofFloatColor c;
    c.setHsb(hue, 1.0, onsetMult);
    
    // On first measure, choose a new palette
    // this logic is handled in RandomPalette
    std::vector<ofFloatColor> pal;
    pal = _RandomPalette();
    
    cout<< buffer->size()<<"\n";
    
    for(int lightIndex = 0; lightIndex < buffer->size(); lightIndex++) {
        
        float normalizedIndex = (float)lightIndex / (buffer->size()-1);
        
        int lightsPerTrailer = buffer->size() / 10;
        
        // XXX this is bad b/c it isn't normalized to number of lights.
        int iNumerator = lightIndex + _spinOffset;
        
        float trailDiminish = (float) (iNumerator % lightsPerTrailer) / lightsPerTrailer;
        trailDiminish = onsetAmount * 1.0 + (1.0 - onsetAmount) * trailDiminish;
        trailDiminish *= trailDiminish;
        
        float thisMult = onsetMult * trailDiminish;
        float thisHue = hue + 0.3 * (1.0 - thisMult);
        if (thisHue < 0.0) {
            thisHue += 1.0;
        }
        thisHue = fmod(thisHue, 1.0);
        
        ofFloatColor thisC;
        thisC.setHsb(thisHue, 1.0, thisMult);
        

        int colorIndex = floor(normalizedIndex * pal.size());
        ofFloatColor palc = pal[colorIndex];
        float o = onsetAmount / 2.0;
        
        cout<<onsetAmount<<"\n";
        
        (*buffer)[lightIndex] = palc;
  
    }
}
