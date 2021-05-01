/*
 * Copyright (c) 2021 Meltytech, LLC
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MARKERSMODEL_H
#define MARKERSMODEL_H

#include <MltProducer.h>

#include <QAbstractItemModel>

class MarkersModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    enum Roles {
        TextRole = Qt::UserRole + 1,
        StartRole,
        EndRole,
        ColorRole,
        NeighborMinRole,
        NeighborMaxRole,
    };

    explicit MarkersModel(QObject* parent = 0);
    virtual ~MarkersModel();

    void load(Mlt::Producer* producer);

protected:
    // Implement QAbstractItemModel
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QModelIndex index(int row, int column = 0, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;
    QHash<int, QByteArray> roleNames() const;

private:
    void updateNeighborsMinMax(const QModelIndex& index);
    int markerCount() const;

    Mlt::Producer* m_producer;
};

#endif // MARKERSMODEL_H
