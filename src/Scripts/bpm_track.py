import sys

import numpy as np
import librosa


def main():
    try:
        file = sys.argv[1]
        y, sr = librosa.load(file, sr=None, mono=True)
        tempo, beats = librosa.beat.beat_track(y=y, sr=sr)
        beats = librosa.frames_to_time(beats, sr=sr)
        print(f'{tempo}')
        for b in beats:
            print(f'{b}')
        sys.exit(0)
    except Exception as e:
        print(e)
        sys.exit(1)


if __name__ == '__main__':
    main()
