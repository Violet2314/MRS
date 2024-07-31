import librosa
import numpy as np

def extract_features(file_path):
    y, sr = librosa.load(file_path)
    
    # 提取MFCC特征
    mfcc = librosa.feature.mfcc(y=y, sr=sr, n_mfcc=13)
    mfcc_mean = np.mean(mfcc, axis=1)
    
    # 提取色度特征
    chroma = librosa.feature.chroma_stft(y=y, sr=sr)
    chroma_mean = np.mean(chroma, axis=1)
    
    # 提取节奏特征
    tempo, _ = librosa.beat.beat_track(y=y, sr=sr)
    
    # 汇总所有特征
    features = np.concatenate((mfcc_mean, chroma_mean, [tempo]))
    
    return features

# 示例：提取特征
file_path = '"D:\qt\MRS\Music\Lanchong - 使一颗心免于哀伤哼唱加长[流萤][知更鸟].mp3"'
features = extract_features(file_path)
print(features)
