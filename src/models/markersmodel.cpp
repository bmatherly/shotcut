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

#include "markersmodel.h"

#include <Logger.h>

#include <QColor>

MarkersModel::MarkersModel(QObject* parent)
    : QAbstractItemModel(parent)
    , m_producer(nullptr)
{
}

MarkersModel::~MarkersModel()
{
}

void MarkersModel::load(Mlt::Producer* producer)
{
qDebug() << producer;
    beginResetModel();
    m_producer = producer;
    mlt_properties producer_properties = MLT_PRODUCER_PROPERTIES(m_producer->get_producer());
    mlt_properties marker_properties = mlt_properties_new();
    mlt_properties_set_data( producer_properties, "markers", marker_properties, 0, (mlt_destructor)mlt_properties_close, NULL );

    mlt_properties marker1_properties = mlt_properties_new();
    mlt_properties_set( marker1_properties, "text", "Marker 1" );
    mlt_properties_set_int( marker1_properties, "start", 100 );
    mlt_properties_set_int( marker1_properties, "end", 150 );
    mlt_properties_set_int( marker1_properties, "color", 0x00ff00 );
    mlt_properties_set_data( marker_properties, "1", marker1_properties, 0, (mlt_destructor)mlt_properties_close, NULL );

    mlt_properties marker2_properties = mlt_properties_new();
    mlt_properties_set( marker2_properties, "text", "Marker 2" );
    mlt_properties_set_int( marker2_properties, "start", 200 );
    mlt_properties_set_int( marker2_properties, "end", 250 );
    mlt_properties_set_int( marker2_properties, "color", 0x0000ff );
    mlt_properties_set_data( marker_properties, "2", marker2_properties, 0, (mlt_destructor)mlt_properties_close , NULL);

    mlt_properties marker3_properties = mlt_properties_new();
    mlt_properties_set( marker3_properties, "text", "Marker 2" );
    mlt_properties_set_int( marker3_properties, "start", 300 );
    mlt_properties_set_int( marker3_properties, "end", 350 );
    mlt_properties_set_int( marker3_properties, "color", 0xff0000 );
    mlt_properties_set_data( marker_properties, "3", marker3_properties, 0, (mlt_destructor)mlt_properties_close, NULL );

    endResetModel();
}

int MarkersModel::markerCount() const
{
    if (!m_producer) {
        return 0;
    }
    mlt_properties markerProperties = (mlt_properties)m_producer->get_data("markers");
    if (!markerProperties) {
        return 0;
    }
    return mlt_properties_count(markerProperties);
}

int MarkersModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return markerCount();
}

int MarkersModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant MarkersModel::data(const QModelIndex& index, int role) const
{
    if (!m_producer) {
        LOG_DEBUG() << "No Producer: " << index.row() << index.column() << role;
        return QVariant();
    }
    mlt_properties markersProperties = (mlt_properties)m_producer->get_data("markers");
    if (!markersProperties) {
        LOG_DEBUG() << "No Markers: " << index.row() << index.column() << role;
        return QVariant();
    }
    if (!index.isValid() || index.column() != 0 || index.row() < 0 || index.row() >= markerCount()) {
        LOG_ERROR() << "Invalid Index: " << index.row() << index.column() << role;
        return QVariant();
    }
    switch (role) {
        case Qt::DisplayRole:
        case TextRole: {
            mlt_properties markerProperties = (mlt_properties)mlt_properties_get_data_at( markersProperties, index.row(), NULL );
            return mlt_properties_get(markerProperties, "text");
        }
        case StartRole: {
            mlt_properties markerProperties = (mlt_properties)mlt_properties_get_data_at( markersProperties, index.row(), NULL );
            return mlt_properties_get_int(markerProperties, "start");
        }
        case EndRole: {
            mlt_properties markerProperties = (mlt_properties)mlt_properties_get_data_at( markersProperties, index.row(), NULL );
            return mlt_properties_get_int(markerProperties, "end");
        }
        case ColorRole: {
            mlt_properties markerProperties = (mlt_properties)mlt_properties_get_data_at( markersProperties, index.row(), NULL );
            return QColor(mlt_properties_get_int(markerProperties, "color"));
        }
        case NeighborMinRole: {
            int minimum = 0;
            if (index.row() > 0) {
                mlt_properties markerProperties = (mlt_properties)mlt_properties_get_data_at( markersProperties, index.row() - 1, NULL );
                minimum = mlt_properties_get_int(markerProperties, "end");
            }
            return minimum;
        }
        case NeighborMaxRole: {
            int maximum = 1000;
            if (index.row() < markerCount() - 1) {
                mlt_properties markerProperties = (mlt_properties)mlt_properties_get_data_at( markersProperties, index.row() + 1, NULL );
                maximum = mlt_properties_get_int(markerProperties, "start");
            }
            return maximum;
        }
        default:
            LOG_ERROR() << "Invalid Role" << index.row() << role;
            break;
    }
    return QVariant();
}

QModelIndex MarkersModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    if (column != 0 || row < 0 || row >= markerCount())
        return QModelIndex();
    return createIndex(row, column, (int)0);
}

QModelIndex MarkersModel::parent(const QModelIndex& index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}

QHash<int, QByteArray> MarkersModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TextRole]        = "text";
    roles[StartRole]       = "start";
    roles[EndRole]         = "end";
    roles[ColorRole]       = "color";
    roles[NeighborMinRole] = "neighborMin";
    roles[NeighborMaxRole] = "neighborMax";
    return roles;
}

void MarkersModel::updateNeighborsMinMax(const QModelIndex& index)
{
    if (index.row() > 0) {
        QModelIndex sibling = index.siblingAtRow(index.row() - 1);
        emit dataChanged(sibling, sibling, QVector<int>() << NeighborMaxRole);
    }
    if (index.row() < markerCount() - 1) {
        QModelIndex sibling = index.siblingAtRow(index.row() + 1);
        emit dataChanged(sibling, sibling, QVector<int>() << NeighborMinRole);
    }
}
