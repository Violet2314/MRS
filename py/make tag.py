import numpy as np
from sklearn.cluster import KMeans
from analyse import extract_features

# 假设我们有多个音频文件的特征
all_features = np.array([
    extract_features('file1.mp3'),
    extract_features('file2.mp3'),
    extract_features('file3.mp3'),
    # 其他文件...
])

# 使用K-means进行聚类
kmeans = KMeans(n_clusters=5)
labels = kmeans.fit_predict(all_features)
print("聚类标签：", labels)
