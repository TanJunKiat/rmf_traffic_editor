/*
 * Copyright (C) 2019 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>

#include "vertex.h"


Vertex::Vertex()
: x(0), y(0), selected(false)
{
}

Vertex::Vertex(double _x, double _y, const std::string &_name)
: x(_x), y(_y), name(_name), selected(false)
{
}

void Vertex::from_yaml(const YAML::Node &data)
{
  if (!data.IsSequence())
    throw std::runtime_error("Vertex::from_yaml expected a sequence");
  x = data[0].as<double>();
  y = data[1].as<double>();
  if (data.size() < 4)
    return;  // todo: remove... intended only during format transition
  // skip the z-offset in data[2] for now
  name = data[3].as<std::string>();
}

YAML::Node Vertex::to_yaml() const
{
  // This is in image space. I think it's safe to say nobody is clicking
  // with more than 1/1000 precision inside a single pixel.

  YAML::Node vertex_node;
  vertex_node.push_back(round(x * 1000.0) / 1000.0);
  vertex_node.push_back(round(y * 1000.0) / 1000.0);
  vertex_node.push_back(0.0);  // placeholder for Z offsets in the future
  vertex_node.push_back(name);
  vertex_node.SetStyle(YAML::EmitterStyle::Flow);
  return vertex_node;
}

void Vertex::draw(
    QGraphicsScene *scene,
    const double meters_per_pixel) const
{
  QPen vertex_pen(Qt::black);
  vertex_pen.setWidth(0.05 / meters_per_pixel);
  const double radius = 0.1 / meters_per_pixel;

  const double a = 0.5;
  QColor color = QColor::fromRgbF(0.0, 1.0, 0.0, a);
  QColor selected_color = QColor::fromRgbF(1.0, 0.0, 0.0, a);

  scene->addEllipse(
      x - radius,
      y - radius,
      2 * radius,
      2 * radius,
      vertex_pen,
      selected ? QBrush(selected_color) : QBrush(color));

  if (!name.empty()) {
    QGraphicsSimpleTextItem *item = scene->addSimpleText(
        QString::fromStdString(name));
    item->setBrush(QColor(255, 0, 0, 255));
    item->setPos(x, y + radius);
  }
}