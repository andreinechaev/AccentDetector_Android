# Speaker app

The application is initial step towards Accent detector

## Road map

1. Read & Write audio simultaneously from/to mic and speaker
    - Support earpiece
    - Support USB headphones
    - Support BT headphones
    - Achieve minimal latency
    
2. Draw FFT in real time
    - Calculate FFT without affecting audio transmitter (multithreading)
    - Display FFT based spectrogram in real time
    - Achieve minimal latency
    
3. Embed tongue twisters
	- UI for selecting twisters
	- UI font and background selection (Themes)
    
4. Speech to text
    -  Use embedded tools for speech recognition
    -  Try word to word processing with confidence level
    -  Colorize text based on confidence level