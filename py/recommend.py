def recommend_by_label(user_liked_labels, all_labels, k=5):
    # 统计用户喜欢的标签
    from collections import Counter
    label_counter = Counter(user_liked_labels)
    
    # 按标签出现频率排序
    most_common_labels = [label for label, count in label_counter.most_common()]
    
    # 推荐相同标签的歌曲
    recommended_songs = []
    for label in most_common_labels:
        recommended_songs.extend([i for i, song_label in enumerate(all_labels) if song_label == label])
    
    return recommended_songs[:k]
