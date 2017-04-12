#pragma once
#include "SharedObjects.hpp"
#include "Field.hpp"

#include "qstring.h"
#include "qwidget.h"
#include "qpushbutton.h"
#include "qgridlayout.h"

template<size_t FieldSize = 4, class Element = QPushButton>
class FieldWidget : public QWidget {
protected:
	Field<FieldSize>* m_field;
	Element* m_elements[FieldSize][FieldSize];
	QGridLayout* m_layout;
public:
	explicit FieldWidget(Field<FieldSize>* f) : m_field(f) {
		m_layout = new QGridLayout();
		for (size_t i = 0; i < FieldSize; i++)
			for (size_t j = 0; j < FieldSize; j++) {
				m_elements[i][j] = new Element(generateValue(m_field->at(i, j)));
				m_elements[i][j]->setMinimumSize(80, 80);
				m_elements[i][j]->setFont(SharedObjects::defaultFont);
				m_elements[i][j]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
				m_layout->addWidget(m_elements[i][j], i, j, 1, 1);
			}
		setLayout(m_layout);
	}
	QString generateValue(short v) {
		return (v == 0) ? QString(" ") : QString::number((size_t) pow(2, v));
	}

	void update() {
		for (size_t i = 0; i < FieldSize; i++)
			for (size_t j = 0; j < FieldSize; j++)
				m_elements[i][j]->setText(generateValue(m_field->at(i, j)));
		QWidget::update();
	}
};