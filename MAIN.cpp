#define _USE_MATH_DEFINES
#include "RtAudio.h"
#include <iostream>
#include <stdio.h>
#include <array>
#include <cstdlib>
#include <cmath>

//Big thanks to the mod documentation by Thunder
//https://ftp.modland.com/pub/documents/format_documentation/Protracker%20effects%20(MODFIL12.TXT)%20(.mod).txt

const std::array<uint16_t, 60 * 16> PeriodTable = {
    1712, 1616, 1524, 1440, 1356, 1280, 1208, 1140, 1076, 1016, 960, 906, //11
        856, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480, 453, //23
        428, 404, 381, 360, 339, 320, 302, 285, 269, 254, 240, 226, //35
        214, 202, 190, 180, 170, 160, 151, 143, 135, 127, 120, 113, //47
        107, 101, 95, 90, 85, 80, 75, 71, 67, 63, 60, 56, //59 (index)
    1700, 1604, 1514, 1430, 1348, 1274, 1202, 1134, 1070, 1010, 954, 900,
        850, 802, 757, 715, 674, 637, 601, 567, 535, 505, 477, 450,
        425, 401, 379, 357, 337, 318, 300, 284, 268, 253, 239, 225,
        213, 201, 189, 179, 169, 159, 150, 142, 134, 126, 119, 113,
        106, 100, 94, 89, 84, 79, 75, 71, 67, 63, 59, 56,
    1688, 1592, 1504, 1418, 1340, 1264, 1194, 1126, 1064, 1004, 948, 894,
        844, 796, 752, 709, 670, 632, 597, 563, 532, 502, 474, 447,
        422, 398, 376, 355, 335, 316, 298, 282, 266, 251, 237, 224,
        211, 199, 188, 177, 167, 158, 149, 141, 133, 125, 118, 112,
        105, 99, 94, 88, 83, 79, 74, 70, 66, 62, 59, 56,
    1676, 1582, 1492, 1408, 1330, 1256, 1184, 1118, 1056, 996, 940, 888,
        838, 791, 746, 704, 665, 628, 592, 559, 528, 498, 470, 444,
        419, 395, 373, 352, 332, 314, 296, 280, 264, 249, 235, 222,
        209, 198, 187, 176, 166, 157, 148, 140, 132, 125, 118, 111,
        104, 99, 93, 88, 83, 78, 74, 70, 66, 62, 59, 55,
    1664, 1570, 1482, 1398, 1320, 1246, 1176, 1110, 1048, 990, 934, 882,
        832, 785, 741, 699, 660, 623, 588, 555, 524, 495, 467, 441,
        416, 392, 370, 350, 330, 312, 294, 278, 262, 247, 233, 220, //350
        208, 196, 185, 175, 165, 156, 147, 139, 131, 124, 117, 110,
        104, 98, 92, 87, 82, 78, 73, 69, 65, 62, 58, 55,
    1652, 1558, 1472, 1388, 1310, 1238, 1168, 1102, 1040, 982, 926, 874,
        826, 779, 736, 694, 655, 619, 584, 551, 520, 491, 463, 437,
        413, 390, 368, 347, 328, 309, 292, 276, 260, 245, 232, 219,
        206, 195, 184, 174, 164, 155, 146, 138, 130, 123, 116, 109,
        103, 97, 92, 87, 82, 77, 73, 69, 65, 61, 58, 54,
    1640, 1548, 1460, 1378, 1302, 1228, 1160, 1094, 1032, 974, 920, 868,
        820, 774, 730, 689, 651, 614, 580, 547, 516, 487, 460, 434,
        410, 387, 365, 345, 325, 307, 290, 274, 258, 244, 230, 217,
        205, 193, 183, 172, 163, 154, 145, 137, 129, 122, 115, 109,
        102, 96, 91, 86, 81, 77, 72, 68, 64, 61, 57, 54,
    1628, 1536, 1450, 1368, 1292, 1220, 1150, 1086, 1026, 968, 914, 862,
        814, 768, 725, 684, 646, 610, 575, 543, 513, 484, 457, 431,
        407, 384, 363, 342, 323, 305, 288, 272, 256, 242, 228, 216,
        204, 192, 181, 171, 161, 152, 144, 136, 128, 121, 114, 108,
        102, 96, 90, 85, 80, 76, 72, 68, 64, 60, 57, 54,
    1814, 1712, 1616, 1524, 1440, 1356, 1280, 1208, 1140, 1076, 1016, 960,
        907, 856, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480,
        453, 428, 404, 381, 360, 339, 320, 302, 285, 269, 254, 240,
        226, 214, 202, 190, 180, 170, 160, 151, 143, 135, 127, 120,
        113, 107, 101, 95, 90, 85, 80, 75, 71, 67, 63, 60,
    1800, 1700, 1604, 1514, 1430, 1350, 1272, 1202, 1134, 1070, 1010, 954,
        900, 850, 802, 757, 715, 675, 636, 601, 567, 535, 505, 477,
        450, 425, 401, 379, 357, 337, 318, 300, 284, 268, 253, 238,
        225, 212, 200, 189, 179, 169, 159, 150, 142, 134, 126, 119,
        112, 106, 100, 94, 89, 84, 79, 75, 71, 67, 63, 59,
    1788, 1688, 1592, 1504, 1418, 1340, 1264, 1194, 1126, 1064, 1004, 948,
        894, 844, 796, 752, 709, 670, 632, 597, 563, 532, 502, 474,
        447, 422, 398, 376, 355, 335, 316, 298, 282, 266, 251, 237,
        223, 211, 199, 188, 177, 167, 158, 149, 141, 133, 125, 118,
        111, 105, 99, 94, 88, 83, 79, 74, 70, 66, 62, 59,
    1774, 1676, 1582, 1492, 1408, 1330, 1256, 1184, 1118, 1056, 996, 940,
        887, 838, 791, 746, 704, 665, 628, 592, 559, 528, 498, 470,
        444, 419, 395, 373, 352, 332, 314, 296, 280, 264, 249, 235,
        222, 209, 198, 187, 176, 166, 157, 148, 140, 132, 125, 118,
        111, 104, 99, 93, 88, 83, 78, 74, 70, 66, 62, 59,
    1762, 1664, 1570, 1482, 1398, 1320, 1246, 1176, 1110, 1048, 988, 934,
        881, 832, 785, 741, 699, 660, 623, 588, 555, 524, 494, 467,
        441, 416, 392, 370, 350, 330, 312, 294, 278, 262, 247, 233,
        220, 208, 196, 185, 175, 165, 156, 147, 139, 131, 123, 117,
        110, 104, 98, 92, 87, 82, 78, 73, 69, 65, 61, 58,
    1750, 1652, 1558, 1472, 1388, 1310, 1238, 1168, 1102, 1040, 982, 926,
        875, 826, 779, 736, 694, 655, 619, 584, 551, 520, 491, 463,
        437, 413, 390, 368, 347, 328, 309, 292, 276, 260, 245, 232,
        219, 206, 195, 184, 174, 164, 155, 146, 138, 130, 123, 116,
        109, 103, 97, 92, 87, 82, 77, 73, 69, 65, 61, 58,
    1736, 1640, 1548, 1460, 1378, 1302, 1228, 1160, 1094, 1032, 974, 920,
        868, 820, 774, 730, 689, 651, 614, 580, 547, 516, 487, 460,
        434, 410, 387, 365, 345, 325, 307, 290, 274, 258, 244, 230,
        217, 205, 193, 183, 172, 163, 154, 145, 137, 129, 122, 115,
        108, 102, 96, 91, 86, 81, 77, 72, 68, 64, 61, 57,
    1724, 1628, 1536, 1450, 1368, 1292, 1220, 1150, 1086, 1026, 968, 914,
        862, 814, 768, 725, 684, 646, 610, 575, 543, 513, 484, 457,
        431, 407, 384, 363, 342, 323, 305, 288, 272, 256, 242, 228,
        216, 203, 192, 181, 171, 161, 152, 144, 136, 128, 121, 114,
        108, 101, 96, 90, 85, 80, 76, 72, 68, 64, 60, 57 };



float TempFreq = 1.0f; //Finally figured this out
float TempAmp = (1.0f/128.0f) * 0.5f;//0.001f; //1.0 is highest
const unsigned int SampleRate = 44100;
const float SRInvert = 1.0f / SampleRate;
float Pointer = 0.0f;
bool ReadyToPlay = false;

const unsigned int SampleTableLength = 64; //Has to be constant -_-
std::array<double, SampleTableLength> SampleTable;

//Mod Globals
char SongName[20];
uint8_t NumOfPatterns = 0;
uint8_t SongEndJump;
std::array<uint8_t,128> PatternTable;
char FileTag[4];
unsigned int NumOfUniquePatterns = 0;
uint8_t Speed = 6;
uint8_t TickNum = 0; //Use mod 6 for this
int TickCounter = 0; // use mod Samplerate * 0.02;
uint8_t CurrentRow = 0; //0 to 63
uint8_t PatternNumber = 0;
uint8_t CurrentPattern = 0; 
int SamplesPerTick = SampleRate * 0.02;
int JumpToNextPat = -1; //>-1 matters
int SkipToPattern = -1;
int DelayPattern = -1;

struct ModSample {
    char SampleName[22];
    unsigned int SampleLength;
    uint8_t SampleVol = 0;
    uint8_t SampleFineTune = 0;
    uint16_t SampleRepeatOffset = 0;
    uint16_t SampleRepeatLength;
    bool ShouldRepeat = false;
    
    //vectors should be the last thing in a struct

    //std::vector<int8_t> SampleData;

};

struct ModPatternNote {
    uint8_t SampleID;
    uint16_t NotePeriod = 0;
    uint16_t EffectAndArg;
};

struct ModChannel { //There'll be four of these, which keep track of what's going on in their column
    bool Playing = false;
    
    float TimeSinceLastTrigger = 0.0f;
    uint8_t ChannelVolume = 64;
    
    int SampleID = 0;
    float SamplePointer = 0.0f;
    uint8_t SampleFineTune = 0;
    uint8_t SampleVol = 0;

    uint16_t LastNotePeriod = 0;
    uint16_t UneffectedPeriod = 0;
    uint16_t CurrentPeriod = 0; //Includes fine tune
    uint16_t NoteNumber = 0;
    float Frequency = 0;


    uint16_t CurrentEffect = 0;
    int8_t CurrentEffValue = 0;
    unsigned int EMem = 0; //Remember what effect was played recently, or similar things
    uint16_t EVMem = 0; //Remember what Value -for whichever effect- was played recently
    uint16_t EVMemTwo = 0;

    //3xx is so important the whole channel needs to know about it
    bool EffectSlide = false;
    uint16_t SlideTarget = 0;

    //4xy
    bool VibUp = true;
    float VibOffset = 0.0f;

    //0xy 
    unsigned int arp = 0;

    //7xy
    bool TremUp = true;
    float TremOffset = 0.0f;

    //8xx
    double ChannelPan = 0.0;

    //E3y
    bool E3Enabled = false;
    uint16_t Real = 0;

    //E6y
    bool E6Looping = false;
    int RowLoop = 0;
    int NumLoops = 0;
};
std::vector<std::array<ModPatternNote, 256>>  PatternData;

std::array<ModSample, 31> SampleHeaders;

std::array<ModChannel, 4> Channels;

std::array<std::vector<int8_t>, 31> PureSampleData; //Its a bit annoying, but I guess this is fine

uint8_t Note[4];
uint16_t NotePeriod = 0;
uint8_t SampleID;
uint16_t EffectAndArg;

uint16_t RoundToTable(uint16_t Note, uint8_t Ft) {
    std::cout << "\nTrying Round to table";
    uint16_t Dist = 100;
    uint16_t Result = Note;
    for (uint16_t i = 0; i < 60; i++) {
        if (std::abs(PeriodTable[i + 60 * Ft] - Note) < Dist) {
            Dist = std::abs(PeriodTable[i + 60 * Ft] - Note);
            Result = PeriodTable[i + 60 * Ft];
            if (Dist == 0) {
                return Result;
            }
        }
    }
    return Result;
}


//EFFECTs FUNCTIONS
void ApplyEffects(int e, uint16_t Type, uint8_t Value) {
    //e is which channel we're on
    if ((Type != 3 && Type != 5) && (Channels[e].EffectSlide == true)) {
        Channels[e].EffectSlide = false;
        //So many accomodations just for this one effect
    }
    if ((Type == 0) && (Value == 0)) { Channels[e].EVMem = 0; return; }
    if (Type != 8) {
        Channels[e].ChannelPan = ((e == 0 || e == 3) ? 0.70 : 0.30);
    }
    if (Type != 6 || Type != 4) {
        Channels[e].VibOffset = 0.0f;
    }
    switch (Type) {
        case 0:
        {
            int x = Value >> 4;
            int y = Value & 0xF;
            if (Channels[e].arp % 3 == 0) {
                Channels[e].CurrentPeriod = Channels[e].UneffectedPeriod;
            }
            if (Channels[e].arp % 3 == 1) {
                Channels[e].CurrentPeriod = Channels[e].UneffectedPeriod + x;
            }
            if (Channels[e].arp % 3 == 2) {
                Channels[e].CurrentPeriod = Channels[e].UneffectedPeriod + y;
            }
            Channels[e].arp++;
        }
            break;
        case 1:
            if (TickNum != 0) {
                Channels[e].CurrentPeriod -= Value;
            }
            break;
        case 2:
            if (TickNum != 0) {
                Channels[e].CurrentPeriod += Value;
            }
            break;
        case 5:
        {
            int x = Value >> 4;
            int y = Value & 0xF;
            if (x > 0 && y > 0) { return; }
            Channels[e].ChannelVolume = Channels[e].ChannelVolume + x - y;
            Channels[e].ChannelVolume = std::max((int8_t)0, std::min((int8_t)64, (int8_t)Channels[e].ChannelVolume));
        }
            if (Channels[e].SamplePointer == 0.0f) { //Prevent retrigger of note
                Channels[e].SamplePointer = Channels[e].TimeSinceLastTrigger;

            }
            if ((Channels[e].SlideTarget == Channels[e].LastNotePeriod) && (TickNum == 0)) { //Signifies that the Slide is happening, but we need to change target
                Channels[e].SlideTarget = Channels[e].CurrentPeriod;
            }
            //I guess the unsigned parts are coming back to bite me in the butt
            if (Channels[e].SlideTarget > Channels[e].CurrentPeriod) {
                Channels[e].CurrentPeriod = std::min((uint16_t)(Channels[e].CurrentPeriod + Channels[e].EVMem), Channels[e].SlideTarget); //Sliiiide down
            }
            if (Channels[e].SlideTarget < Channels[e].CurrentPeriod) {
                Channels[e].CurrentPeriod = std::max((int16_t)(Channels[e].CurrentPeriod - Channels[e].EVMem), (int16_t)Channels[e].SlideTarget); // Or sliiide up
                //The type conversion in intentional
            }
            break;
        case 3:
            if (Channels[e].EffectSlide == false) { //First time 3xy is noticed
                Channels[e].EffectSlide = true;
                Channels[e].SlideTarget = Channels[e].CurrentPeriod;
                Channels[e].CurrentPeriod = Channels[e].LastNotePeriod;
            }
            if (Channels[e].SamplePointer == 0.0f) { //Prevent retrigger of note
                Channels[e].SamplePointer = Channels[e].TimeSinceLastTrigger;

            }
            if ((Channels[e].SlideTarget == Channels[e].LastNotePeriod) && (TickNum == 0)) { //Signifies that the Slide is happening, but we need to change target
                Channels[e].SlideTarget = Channels[e].CurrentPeriod;
            }
            if (Value != 0) {
                Channels[e].EVMem = Value; //remember the value, but only if it changes
            }
            //I guess the unsigned parts are coming back to bite me in the butt
            if (Channels[e].SlideTarget > Channels[e].CurrentPeriod) {
               
               if (Channels[e].E3Enabled) {
                   Channels[e].Real = std::min((uint16_t)(Channels[e].CurrentPeriod + Channels[e].EVMem), Channels[e].SlideTarget);
                   Channels[e].CurrentPeriod = RoundToTable(Channels[e].Real, Channels[e].SampleFineTune);
               }
               else {
                   Channels[e].CurrentPeriod = std::min((uint16_t)(Channels[e].CurrentPeriod + Channels[e].EVMem), Channels[e].SlideTarget); //Sliiiide down
               }
            }
            if (Channels[e].SlideTarget < Channels[e].CurrentPeriod) {
                
                if (Channels[e].E3Enabled) {
                    Channels[e].Real = std::max((int16_t)(Channels[e].CurrentPeriod - Channels[e].EVMem), (int16_t)Channels[e].SlideTarget);
                    Channels[e].CurrentPeriod = RoundToTable(Channels[e].Real, Channels[e].SampleFineTune);
                }
                else {
                    Channels[e].CurrentPeriod = std::max((int16_t)(Channels[e].CurrentPeriod - Channels[e].EVMem), (int16_t)Channels[e].SlideTarget); // Or sliiide up
                    //The type conversion in intentional
                }
            }
            break;
        case 4:
        {
            int x = Value >> 4;//how fast
            int y = Value & 0xF;//How high
            if (x != 0 || y != 0) {
                Channels[e].EVMem = x; //remember the value, but only if it changes
                Channels[e].EVMemTwo = y;
            }
            //Assume triangle for now
            if (Channels[e].VibUp) {
                Channels[e].VibOffset = std::min(Channels[e].VibOffset + (float)Channels[e].EVMemTwo * ((float)Channels[e].EVMem) / 64.0f, (float)Channels[e].EVMemTwo);
            }
            else {
                //Subtract
                Channels[e].VibOffset = std::max(Channels[e].VibOffset - (float)Channels[e].EVMemTwo * ((float)Channels[e].EVMem) / 64.0f, (float)-Channels[e].EVMemTwo);
            }
            if (Channels[e].VibOffset == Channels[e].EVMemTwo || Channels[e].VibOffset == -Channels[e].EVMemTwo) {
                Channels[e].VibUp = !Channels[e].VibUp;
            }
        }
        break;
        case 6:
        {
            int x = Value >> 4;
            int y = Value & 0xF;
            if (x > 0 && y > 0) { return; }
            Channels[e].ChannelVolume = Channels[e].ChannelVolume + x - y;
            Channels[e].ChannelVolume = std::max((int8_t)0, std::min((int8_t)64, (int8_t)Channels[e].ChannelVolume));
            //Assume triangle for now
            if (Channels[e].VibUp) {
                Channels[e].VibOffset = std::min(Channels[e].VibOffset + (float)Channels[e].EVMemTwo * ((float)Channels[e].EVMem) / 64.0f, (float)Channels[e].EVMemTwo);
            }
            else {
                //Subtract
                Channels[e].VibOffset = std::max(Channels[e].VibOffset - (float)Channels[e].EVMemTwo * ((float)Channels[e].EVMem) / 64.0f, (float)-Channels[e].EVMemTwo);
            }
            if (Channels[e].VibOffset == Channels[e].EVMemTwo || Channels[e].VibOffset == -Channels[e].EVMemTwo) {
                Channels[e].VibUp = !Channels[e].VibUp;
            }
        }
        break;
        case 7:
        {
            int x = Value >> 4;//how fast
            int y = Value & 0xF;//How high
            if (x != 0 || y != 0) {
                Channels[e].EVMem = x; //remember the value, but only if it changes
                Channels[e].EVMemTwo = y;
            }
            if (Channels[e].TremUp) {
                Channels[e].TremOffset = std::min(Channels[e].TremOffset + (float)Channels[e].EVMemTwo * ((float)Channels[e].EVMem) / 64.0f, (float)Channels[e].EVMemTwo);
            }
            else {
                //Subtract
                Channels[e].TremOffset = std::max(Channels[e].TremOffset - (float)Channels[e].EVMemTwo * ((float)Channels[e].EVMem) / 64.0f, (float)-Channels[e].EVMemTwo);
            }
            if (Channels[e].TremOffset == Channels[e].EVMemTwo || Channels[e].TremOffset == -Channels[e].EVMemTwo) {
                Channels[e].TremUp = !Channels[e].TremUp;
            }
            Channels[e].ChannelVolume += Channels[e].TremOffset;
        }
        break;
        case 8:
            Channels[e].ChannelPan = (double)Value / 255.0;

        break;
        case 9:
            if (TickNum != 0) { return; }
            if ((int)Value * 0x100 >= SampleHeaders[Channels[e].SampleID].SampleLength) { return; }
            Channels[e].SamplePointer = (float)Value * 256.0;
            break;
        case 10:
        {
            int x = Value >> 4;
            int y = Value & 0xF;
            if (x > 0 && y > 0) { return; }
            Channels[e].ChannelVolume = Channels[e].ChannelVolume + x - y;
            Channels[e].ChannelVolume = std::max((int8_t)0, std::min((int8_t)64, (int8_t)Channels[e].ChannelVolume));
        }
        break;
        case 11:
            if (Value > NumOfPatterns) { Value = 0; }
            SkipToPattern = Value;
        break;
        case 13:
        {
            int x = Value >> 4;
            int y = Value & 0xF;
            JumpToNextPat = x * 10 + y;
        }
        break;
        case 12:
            if (TickNum != 0) { return; }
            if (Value > 64) { Value = 64; }
            Channels[e].ChannelVolume = Value;
            break;
        case 15:
            if (TickNum != 0) { return; }
            if ((Value > 0) && (Value < 32)) {
                Speed = Value;
            } else {
                SamplesPerTick = (1.0f / (float)Value) * 2.5f * SampleRate; //Should be the right bpm calculation
            }
            break;
        case 14: //E
        {
            int x = Value >> 4;
            int y = Value & 0xF;
            switch (x) {
            case 1: //E1y
                if (TickNum == 0) {
                    Channels[e].CurrentPeriod -= y;
                }
                break;
            case 2:
                if (TickNum == 0) {
                    Channels[e].CurrentPeriod += y;
                }
                break;
            case 3:
                if (TickNum != 0) { return; }
                if (y > 1) { return; }
                if (y == 1) { Channels[e].E3Enabled = true; }
                if (y == 0) { Channels[e].E3Enabled = false; }
                break;
            case 5:
                if (TickNum != 0) { return; }
                Channels[e].SampleFineTune = (int8_t)y;
                Channels[e].CurrentPeriod = PeriodTable[Channels[e].NoteNumber + 60 * Channels[e].SampleFineTune];
                //SampleHeaders[Channels[e].SampleID].SampleFineTune = (int8_t)y;
            case 6:
                if (TickNum != 0) { return; }
                if (Channels[e].E6Looping && Channels[e].NumLoops > 0) {
                    CurrentRow = Channels[e].RowLoop;
                    Channels[e].NumLoops--;
                } 
                if (!Channels[e].E6Looping) {
                    if (y == 0) {
                        Channels[e].RowLoop = CurrentRow;
                    }
                    else {
                        Channels[e].E6Looping = true;
                        Channels[e].NumLoops = y - 1;
                        CurrentRow = Channels[e].RowLoop;
                    }
                }
            case 9:
                if (TickNum == y) {
                    Channels[e].SamplePointer = 0.0f;
                }
            case 10:
                if (TickNum != 0) { return; }
                Channels[e].ChannelVolume = std::min((int8_t)64, (int8_t)(Channels[e].ChannelVolume + y));
            case 11:
                if (TickNum != 0) { return; }
                Channels[e].ChannelVolume = std::max((int8_t)0, (int8_t)(Channels[e].ChannelVolume - y));
            case 12:
                if (TickNum == y) {
                    Channels[e].ChannelVolume = (int8_t)0;
                }
            case 13:
                if (TickNum == 0) {
                    Channels[e].EVMem = Channels[e].ChannelVolume;
                    Channels[e].ChannelVolume = (int8_t)0;
                }
                if (TickNum == y) {
                    Channels[e].ChannelVolume = Channels[e].EVMem;
                    Channels[e].SamplePointer = 0.0f;
                }
            case 14:
                if (TickNum != 0) { return; }
                if (y > 0) {
                    DelayPattern = (int)Speed;
                    Speed = (uint8_t)y * (uint8_t)Speed;
                }
            }
        }
            break;
        default:
            break;
    }
}



//END EFFECTS FUNCTION
bool db = false;

int saw(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
    double streamTime, RtAudioStreamStatus status, void* userData) {
    unsigned int i, j;
        double* buffer = (double*)outputBuffer; //--Directions to one (it's actually more) double
        double* lastValues = (double*)userData; //--Userdata is just a 2 piece array of doubles
        if (status)
            std::cout << "Stream underflow detected!" << std::endl;
        for (i = 0; i < nBufferFrames; i++) { //Since a new note could come at any time, we have to do this
            double Left = 0;
            double Right = 0;


            //First read notes, indentify effects, set relevant variables
            for (int e = 0; e < 4; e++) {
                if (TickCounter != 0 || TickNum != 0) { break; } //I think we only need to do this on the start of each row

                //Check for pattern break (Dxy) here
                if (JumpToNextPat > -1) {
                    PatternNumber++;
                    CurrentPattern = PatternTable[PatternNumber];
                    std::cout << "New Pattern: " << (int)CurrentPattern;
                    CurrentRow = JumpToNextPat;
                    JumpToNextPat = -1;
                    //saw(outputBuffer, inputBuffer, nBufferFrames,
                        //streamTime, status, userData); //This actually causes the break in pattern
                }
                //Check for pattern skip (Bxx) here
                if (SkipToPattern > -1) {
                    PatternNumber = SkipToPattern;
                    CurrentPattern = PatternTable[PatternNumber];
                    std::cout << "New Pattern: " << (int)CurrentPattern;
                    CurrentRow = 0;
                    SkipToPattern = -1;
                }
                ModPatternNote& N = PatternData.at(CurrentPattern).at(
                    (uint8_t)4 * CurrentRow + e
                );
                Channels[e].CurrentEffect = N.EffectAndArg >> 8; 
                Channels[e].CurrentEffValue = N.EffectAndArg & 0xFF;
                if (N.NotePeriod != 0) { //The absolute first time a new note is read, not before or after
                    if (!(N.SampleID == 255)) {
                        db = false;
                        Channels[e].Playing = true;
                        Channels[e].SampleID = N.SampleID;
                        Channels[e].TimeSinceLastTrigger = Channels[e].SamplePointer; //Useful!
                        Channels[e].SamplePointer = 0;
                        Channels[e].ChannelVolume = SampleHeaders[N.SampleID].SampleVol;
                        Channels[e].SampleVol = SampleHeaders[N.SampleID].SampleVol;//We'll see if this is useful
                        Channels[e].SampleFineTune = SampleHeaders[N.SampleID].SampleFineTune;
                    }
                    Channels[e].LastNotePeriod = Channels[e].CurrentPeriod;
                    Channels[e].NoteNumber = N.NotePeriod;
                    Channels[e].CurrentPeriod = PeriodTable[N.NotePeriod + 60 * Channels[e].SampleFineTune];
                    Channels[e].UneffectedPeriod = Channels[e].CurrentPeriod;
                }
                
            } //Done


            //Next update channel playing requirements (freq, effects, ect)
            for (int e = 0; e < 4; e++) {
                if (TickCounter != 0) { break; } //This should be done every tick, at the very most
                
                ApplyEffects(e, Channels[e].CurrentEffect, Channels[e].CurrentEffValue);


                Channels[e].Frequency = 7159090.0 / (2.0f * (float)(
                    //PeriodTable[Channels[e].CurrentPeriod + 60 * Channels[e].SampleFineTune]
                    Channels[e].CurrentPeriod + Channels[e].VibOffset
                    ));  
                Channels[e].Frequency /= SampleRate;
                
                
            }


            
            //Now add to Right/left channels
            for (int e = 0; e < 4; e++) {
                double out = 0;
                if (Channels[e].Playing) {
                    
                    ModSample& SH = SampleHeaders[Channels[e].SampleID];
                    std::vector<int8_t>& Data = PureSampleData[Channels[e].SampleID];
                    /*
                    if (e == 1) {
                        if ((Channels[e].SampleID == 5) && !db) {
                            db = true;
                            printf("\nIt's 5");
                            Data = PureSampleData[5];
                        }
                    }
                    */

                    //Linear Interp stuff
                    int Below = (int)(Channels[e].SamplePointer);
                    int Above = Below + 1;
                    if (Above >= SH.SampleLength) { Above = SH.SampleRepeatOffset; }
                    float FracA = Channels[e].SamplePointer - Below;


                    out = TempAmp * ((1.0f - FracA) * Data[Below] + FracA * Data[Above])
                        * (Channels[e].ChannelVolume * 0.015625);//Number is 1/64, because 64 possible volumes


                    Channels[e].SamplePointer += Channels[e].Frequency;
                    while (Channels[e].SamplePointer >= SH.SampleLength) {
                        Channels[e].SamplePointer -= SH.SampleRepeatLength;
                        if (!SH.ShouldRepeat) {
                            Channels[e].Playing = false;
                        }
                    }

                    /*
                    Right += (((e == 0 || e == 3) ? 0.30 : 0.70)
                        * out);
                    Left += (((e == 0 || e == 3) ? 0.70 : 0.30)
                        * out);
                    */
                    Right += (1.0 - Channels[e].ChannelPan) * out;
                    Left += Channels[e].ChannelPan * out;
                }

            }

            //Increment ticks and update row/pattern info
            TickCounter++;
            if (TickCounter >= SamplesPerTick) { //882 is how many samples per tick (normally)
                TickCounter = 0;
                TickNum = (TickNum + 1) % Speed;
                if (TickNum == 0) {
                    if (DelayPattern > -1) {
                        Speed = (uint8_t)DelayPattern;
                        DelayPattern = -1;
                    }
                    CurrentRow = (CurrentRow + 1) % 64; //Number of rows in a mod
                    if (CurrentRow == 0) {
                        PatternNumber++;
                        for (int e = 0; e < 4; e++) { //Here because of E6Y
                            Channels[e].RowLoop = 0;
                            Channels[e].E6Looping = false;
                        }
                        CurrentPattern = PatternTable[PatternNumber];
                        std::cout << "New Pattern: " << (int)CurrentPattern;
                    }
                }
            }

        //Add interleaved data, final thing
        *buffer = Left;
        buffer++;
        *buffer = Right;
        buffer++;
        }
        return 0;
}

int main()
{
    Channels[0].ChannelPan = 0.7;
    Channels[1].ChannelPan = 0.3;
    Channels[2].ChannelPan = 0.7;
    Channels[3].ChannelPan = 0.3;
    std::cout << "\nHowdy\n";
    //Read the mod file in the AudioStuff Folder
    errno_t err;
    FILE* FilePoint;
    err = fopen_s(&FilePoint,"./AudioStuff/kooky.mod", "rb"); //rb is read binary
    //^Why does nobody seem to consider how unintuitive path directories are^?
    //This is a 31 sample mod, I checked
    if (err == 0)
    {
        std::cout << "\nIt worked apparently\n";


        fseek(FilePoint, 0, SEEK_END); //--Sets the file pointer(where it's looking) to 0 bytes from SEEK_END(the end)
        const uint32_t fileSize = (uint32_t)ftell(FilePoint); //--ftell returns the pointer's position
        rewind(FilePoint); //--set file pointer back to the start

        std::cout << fileSize << "\n\n";

        int reader = fread_s(SongName, sizeof(SongName), sizeof(char), 20, FilePoint);
        if (reader != 0) {
            std::cout << "Song Name: " << SongName << "\n";
        }

        for (int w = 0; w < 31; w++) { //SAMPLE HEADERS

            reader = fread_s(SampleHeaders[w].SampleName, sizeof(SampleHeaders[w].SampleName), sizeof(char), 22, FilePoint);
            if (reader != 0) {
                std::cout << "Sample "<< w << " Name: " << SampleHeaders[w].SampleName << "\n";
            }

            unsigned char b[2];
            reader = fread_s(&b, sizeof(b), sizeof(uint16_t), 1, FilePoint);
            if (reader != 0) {
                SampleHeaders[w].SampleLength = (uint16_t)b[1] | (uint16_t)b[0] << 8; //Account for big endian garbage
                SampleHeaders[w].SampleLength *= 2;
                std::cout << "Sample " << w << " Length (in 1 byte ints): " << SampleHeaders[w].SampleLength << "\n";
            }

            reader = fread_s(&(SampleHeaders[w].SampleFineTune), sizeof(SampleHeaders[w].SampleFineTune), sizeof(int8_t), 1, FilePoint);
            if (reader != 0) {
               // if (SampleHeaders[w].SampleFineTune >= 8) {
               //     SampleHeaders[w].SampleFineTune -= 16;
               // }
                std::cout << "Sample " << w << " fine tune: " << (int)SampleHeaders[w].SampleFineTune << "\n";
            }

            reader = fread_s(&(SampleHeaders[w].SampleVol), sizeof(SampleHeaders[w].SampleVol), sizeof(uint8_t), 1, FilePoint);
            if (reader != 0) {
                std::cout << "Sample " << w << " Vol: " << (int)SampleHeaders[w].SampleVol << "\n";
            }

            reader = fread_s(&b, sizeof(b), sizeof(uint16_t), 1, FilePoint);
            if (reader != 0) {
                SampleHeaders[w].SampleRepeatOffset = (uint16_t)b[1] | (uint16_t)b[0] << 8; //Account for big endian garbage
                SampleHeaders[w].SampleRepeatOffset *= 2;
                std::cout << "Sample " << w << " Repeat Offset (in bytes): " << (int)SampleHeaders[w].SampleRepeatOffset << "\n";
            }

            reader = fread_s(&b, sizeof(b), sizeof(uint16_t), 1, FilePoint);
            if (reader != 0) {
                SampleHeaders[w].SampleRepeatLength = (uint16_t)b[1] | (uint16_t)b[0] << 8; //Account for big endian garbage
                SampleHeaders[w].SampleRepeatLength *= 2;
                std::cout << "Sample " << w << " Repeat Length (in bytes): " << (int)SampleHeaders[w].SampleRepeatLength << "\n";
                if (SampleHeaders[w].SampleRepeatLength > 2) {
                    SampleHeaders[w].ShouldRepeat = true;
                }
            }

        }

        std::cout << "\n";
        //Pattern header info
        fseek(FilePoint, 950, SEEK_SET);

        reader = fread_s(&NumOfPatterns, sizeof(NumOfPatterns), sizeof(uint8_t), 1, FilePoint);
        if (reader != 0) {
            std::cout << "Number of patterns: " << (int)NumOfPatterns << "\n";
        }

        reader = fread_s(&SongEndJump, sizeof(SongEndJump), sizeof(uint8_t), 1, FilePoint);
        if (reader != 0) {
            std::cout << "Song End jump position: " << (int)SongEndJump << "\n"; //So it says it should end, but in most programs it loops
        }

        reader = fread_s(&PatternTable, sizeof(PatternTable), sizeof(char), 128, FilePoint);
        if (reader != 0) {
            std::cout << "Pattern Table: ";
            for (int w = 0; w < NumOfPatterns; w++) {
                std::cout << (int)PatternTable[w] << " | ";
            }
            std::cout << "\n\n";
        }

        reader = fread_s(&FileTag, sizeof(FileTag), sizeof(char), 4, FilePoint);
        if (reader != 0) {
            std::cout << "What's in the file tag: " << FileTag << "\n";
        }

        //Find out the total number of patterns
        int highest = 0;
        for (int w = 0; w < 127; w++) {
            if (PatternTable[w] > highest) {
                highest = PatternTable[w];
            }
        }

        std::cout << "Highest found pattern: " << highest << "\n";
        std::cout << "Total number of unique patterns: " << highest + 1 << "\n";
        NumOfUniquePatterns = highest + 1;

        //Get to the pattern data
        fseek(FilePoint, 1084, SEEK_SET);

        PatternData.resize(NumOfUniquePatterns);
        for (int w = 0; w < NumOfUniquePatterns; w++) {
            for (int j = 0; j < 256; j++) { //256 is the number of notes (sampleid period and effect) per pattern
                reader = fread_s(Note, sizeof(Note), sizeof(char), 4, FilePoint);
                if (reader != 0) {
                    uint16_t N = ((Note[0] & 0x0F) << 8) | Note[1];
                    for (uint16_t i = 0; i < 60; i++) {
                        if (N >= PeriodTable[i]) {
                            PatternData.at(w).at(j).NotePeriod = i;
                            break;
                        }
                    }
                    //PatternData.at(w).at(j).NotePeriod = ((Note[0] & 0x0F) << 8) | Note[1];//Got this from pmplay.c
                    PatternData.at(w).at(j).SampleID = ((Note[0] & 0xF0) | (Note[2] >> 4)) - 1;
                    PatternData.at(w).at(j).EffectAndArg = ((uint16_t)(Note[2] & 0x0F) << 8) | ((uint16_t)Note[3]); //Seems like this works!
                }
            }
        }

       std::cout << "\nPattern Data Loaded Hopefully\n\n";
        std::cout << "What's this note period: " << PatternData.at(2).at(160).NotePeriod << "\n";
        TempFreq = 7159090.0 / (2.0f * (float)PatternData.at(2).at(160).NotePeriod);
        TempFreq /= SampleRate;
        std::cout << "What's this note ID: " << (int)PatternData.at(2).at(160).SampleID << "\n";
        std::cout << "What's this effect and arg: " << PatternData.at(2).at(160).EffectAndArg << "\n";

        //Get to the sample data
        for (int i = 0; i < 31; i++) {
            if (SampleHeaders[i].SampleLength == 0) { continue; }
            PureSampleData[i].resize(SampleHeaders[i].SampleLength);
            for (int w = 0; w < SampleHeaders[i].SampleLength; w++) {
                int8_t temp = 0;
                reader = fread_s(&temp, sizeof(temp), sizeof(int8_t), 1, FilePoint);
                PureSampleData[i].at(w) = temp;
            }
            std::cout << "-Sample " << i << " data hopefully loaded-\n";
            std::cout << "Sample " << i << " data size: " << PureSampleData[i].size() << "\n";
        }

        err = fclose(FilePoint);
        if (err == 0)
        {
            printf("The file was closed\n");
            CurrentPattern = PatternTable[0];
            ReadyToPlay = true;
        }
    }
    else {
        std::cout << err;
    }

    if (!ReadyToPlay) { return 0; }

    RtAudio dac;
    dac.showWarnings(true);
    if (dac.getDeviceCount() < 1) {
        std::cout << "\nNo audio devices found!\n";
        exit(0);
    }
    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice();
    parameters.nChannels = 2;
    parameters.firstChannel = 0;
    unsigned int sampleRate = SampleRate;
    unsigned int bufferFrames = 512; // 256 sample frames --If two channels this means 256 * 2 data points, interleaved
    double data[2];

    try {
        dac.openStream(&parameters, NULL, RTAUDIO_FLOAT64,
            sampleRate, &bufferFrames, &saw, (void*)&data);
        dac.startStream();
    }
    catch (RtAudioError& e) {
        e.printMessage();
        exit(0);
    }

    char input;
    std::cout << "\nPlaying ... press <enter> to quit.\n";
    std::cin.get(input);
    try {
        // Stop the stream
        dac.stopStream();
    }
    catch (RtAudioError& e) {
        e.printMessage();
    }
    if (dac.isStreamOpen()) dac.closeStream();
    PeriodTable[151];
    return 0;
}

