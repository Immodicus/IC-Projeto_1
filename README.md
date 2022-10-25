# Information and codification (IC) - Project 1

---

## Group members - G11

|  NMec | Name                | email                   |
| ----: | ------------------- | ----------------------- |
| 97147 | Jodionísio Muachifi | jodionsiomuachifi@ua.pt |
| 97688 | Rúben Castelhano    | rubencastelhano@ua.pt   |
| 93413 | Tiago Rodrigues     | rodriguestiago@ua.pt    |

## To build:

make

## To test:

- cd build
- ./wav_hist -w histFileName ../sample.wav 0 // outputs the histogram of channel 0 (left)
- .wav_quant -mode round -b 8 ../sample.wav wav_quant_out_round.wav // you can also use audio 02 to test it
- ./wav_cmp ../sample.wav wav_quant_out_round.wav // comparing original and quantized audio file to get SNR and absolute Error
- ./wav_effects -ef singleEcho -g 0.75 -d 0.5 -fc 500 ../sample.wav wav_effects_out_singleEcho.wav //you can also use audio 06 to test it
- ./text_codec -mode encode infile outfile // create a file with some bits to test it
- ./wav_dct_codec -mode enc -bs 1024 -frac 0.9999 -b 16 ../sample.wav wav_dct_codec_out.wav // generates a DCT "compressed" version
