#include "BuffModel.h"

#include "Character.h"
#include "EnabledBuffs.h"
#include "ExternalBuff.h"
#include "Faction.h"
#include "GeneralBuffs.h"

BuffModel::BuffModel(const Content::Phase phase, QObject* parent)
    : QAbstractListModel(parent),
      pchar(nullptr),
      phase(phase)
{}

void BuffModel::set_character(Character* pchar) {
    this->pchar = pchar;
    update_buffs();
}

void BuffModel::set_phase(const Content::Phase phase) {
    this->phase = phase;
    update_buffs();
}

void BuffModel::toggle_buff(const QString& name) {
    QVector<ExternalBuff*> buffs = pchar->get_enabled_buffs()->get_general_buffs()->get_external_buffs();
    for (int i = 0; i < buffs.size(); ++i) {
        if (buffs[i]->get_name() != name)
            continue;

        pchar->get_enabled_buffs()->get_general_buffs()->toggle_external_buff(name);
        break;
    }

    dataChanged(createIndex(0, 0), createIndex(rowCount(), 0), {ActiveRole});
}

void BuffModel::update_buffs() {
    if (!external_buffs.empty()) {
        beginResetModel();
        external_buffs.clear();
        endResetModel();
    }

    QVector<ExternalBuff*> buffs = pchar->get_enabled_buffs()->get_general_buffs()->get_external_buffs();
    for (auto buff : buffs) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        external_buffs << buff;
        endInsertRows();
    }
}

int BuffModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return external_buffs.count();
}

QVariant BuffModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= external_buffs.count())
        return QVariant();

    const ExternalBuff* buff = external_buffs[index.row()];

    if (role == NameRole)
        return buff->get_name();
    if (role == IconRole)
        return buff->get_icon();
    if (role == DescriptionRole)
        return buff->get_description();
    if (role == ActiveRole)
        return pchar->get_enabled_buffs()->get_general_buffs()->buff_active(buff->get_name());

    return QVariant();
}

QHash<int, QByteArray> BuffModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[IconRole] = "icon";
    roles[DescriptionRole] = "description";
    roles[ActiveRole] = "_active";
    return roles;
}
