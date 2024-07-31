#ifndef IMAGESLIDERLABEL_H
#define IMAGESLIDERLABEL_H

#include <QLabel>
#include <QTimer>
#include <vector>

class ImageSliderLabel : public QLabel {
    Q_OBJECT

public:
    ImageSliderLabel(const std::vector<QString>& imagePaths, int interval = 2000, QWidget* parent = nullptr);

private slots:
    void updateImage();

private:
    QTimer* timer;
    std::vector<QString> imagePaths;
    int currentIndex;
};

#endif // IMAGESLIDERLABEL_H
