#include "transaction.h"
#include "eosbytewriter.h"

Transaction::Transaction()
{

}

void Transaction::serialize(EOSByteWriter *writer) const
{
    if (writer) {
        TransactionHeader::serialize(writer);

        SerializeCollection(context_free_action, writer);
        SerializeCollection(actions, writer);
        //writer->putCollection(context_free_action);
        //writer->putCollection(actions);
    }
}

QJsonValue Transaction::toJson() const
{
    QJsonObject obj = TransactionHeader::toJson().toObject();
    QJsonArray ctxFreeActionsArr, actionArr;
    for (const auto& ctx : context_free_action) {
        ctxFreeActionsArr.append(ctx.toJson());
    }

    for (const auto& a : actions) {
        actionArr.append(a.toJson());
    }

    obj.insert("context_free_actions", ctxFreeActionsArr);
    obj.insert("actions", actionArr);

    return QJsonValue(obj);
}

void Transaction::fromJson(const QJsonValue &value)
{
    QJsonObject obj = value.toObject();
    if (obj.isEmpty()) {
        return;
    }

    QJsonArray ctxFreeActionsArr = obj.value("context_free_actions").toArray();
    if (!ctxFreeActionsArr.isEmpty()) {
        for (int i = 0; i < ctxFreeActionsArr.size(); ++i) {
            Action action;
            action.fromJson(ctxFreeActionsArr.at(i));
            context_free_action.push_back(action);
        }
    }

    QJsonArray actionsArr = obj.value("actions").toArray();
    if (!actionsArr.isEmpty()) {
        for (int i = 0; i < actionsArr.size(); ++i) {
            Action action;
            action.fromJson(actionsArr.at(i));
            actions.push_back(action);
        }
    }

    TransactionHeader::fromJson(value);
}

void Transaction::addAction(const Action& action)
{
    actions.push_back(action);
}

void Transaction::setActions(const std::vector<Action> &actions)
{
    this->actions = actions;
}
