# Information and codification (IC) - Project 1

---

## Group members - G11

|  NMec | Name                | email                   |
| ----: | ------------------- | ----------------------- |
| 97147 | Jodionísio Muachifi | jodionsiomuachifi@ua.pt |
| 97688 | Rúben Castelhano    | rubencastelhano@ua.pt   |
| 93413 | Tiago Rodrigues     | rodriguestiago@ua.pt    |

## Building:

- git clone git@github.com:Immodicus/IC-Projeto_1.git
- cd IC-Projeto_1
- mkdir build && cd build
- cmake..
- make

## Testing:

### Ex2.
`./wav_hist` -w histFileName wavFile channel 

#### Example:
- `./wav_hist` -w leftChannel.hist sample.wav 0 // write left channel histogram to file leftChannel.hist which can be viewed with Histogram.m
- `./wav_hist` sample.wav 0 // write left channel histogram to stdout

### Ex3.
`./wav_quant` -mode [midriser|midtread|shift] -b numBits wavFile quantizedFile

#### Example:
- `./wav_quant` -mode midriser -b 12 sample.wav quantizedSample.wav // quantize sample.wav with 12 bits per sample and write to quantizedSample.wav

### Ex4.
`./wav_cmp` wavFile quantizedFile

#### Example:
- `./wav_cmp` sample.wav quantizedSample.wav

### Ex5.
`./wav_effects` -ef [singleEcho|multEcho|am] -g gain -d delay -fc carrierFrequency wavFile effectsFile

#### Example:
- `./wav_effects` -ef singleEcho -g 0.8 -d 0.75 sample.wav singleEcho.wav // create a single echo on sample.wav with 80% gain and 0.75 seconds of delay
- `./wav_effects` -ef am -freq 600 sample.wav am.wav // amplitude modulate sample.wav to a new file with a carrier frequency of 600Hz

### Ex7.
`./text_codec` -mode [encode|decode] inFile outFile

#### Examples:
- `./text_codec` -mode encode in.txt out.bin // encode text file in.txt to binary file out.bin
- `./text_codec` -mode decode out.bin out.txt // decode encoded file out.bin to text file out.txt

### Ex8.

`./wav_dct_codec` -v [1|0] -mode [enc|dec] -bs blockSize -frac energyFrac -b nBits inFile outFile

#### Examples:
- `./wav_dct_codec` -mode enc -bs 1024 -frac 0.999 -b 16 sample.wav encoded.bin // encode audio file sample.wav to binary file encoded.bin in blocks of 1024 frames mantaining 99.9% of the original signal energy and with 16 bit coefficients
- `./wav_dct_codec` -mode dec encoded.bin decoded.wav // decode previously encoded file encoded.bin to audio file decoded.wav