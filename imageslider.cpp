#include "ImageSlider.h"
#include <QPixmap>

ImageSlider:ImageSlider(const std::vector<QString>& imagePaths, int interval, QWidget* parent)
    : QLabel(parent), imagePaths(imagePaths), currentIndex(0) {

    setAlignment(Qt::AlignCenter);

    if (!imagePaths.empty()) {
        setPixmap(QPixmap(imagePaths[0]));
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ImageSliderLabel::updateImage);
    timer->start(interval);
}

void ImageSliderLabel::updateImage() {
    currentIndex = (currentIndex + 1) % imagePaths.size();
    setPixmap(QPixmap(imagePaths[currentIndex]));
}
