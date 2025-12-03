#include "Footer.h"
#include <QHBoxLayout>
#include <QLabel>

Footer::Footer(QWidget *parent)
    : QWidget(parent)
{
    // TO DO: update the footer with terms and conditions
    QHBoxLayout* layout = new QHBoxLayout(this);

    QLabel* label = new QLabel("FOOTER : Zone fixe", this);
    layout->addWidget(label);

    layout->addStretch();
}
