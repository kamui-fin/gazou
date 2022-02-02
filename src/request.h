#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
/*
 * Proxy class for interface org.freedesktop.portal.Request
 */
class OrgFreedesktopPortalRequestInterface : public QDBusAbstractInterface {
    Q_OBJECT
  public:
    static inline const char *staticInterfaceName() {
        return "org.freedesktop.portal.Request";
    }

  public:
    OrgFreedesktopPortalRequestInterface(const QString &service,
                                         const QString &path,
                                         const QDBusConnection &connection,
                                         QObject *parent = nullptr);

    ~OrgFreedesktopPortalRequestInterface();

  public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> Close() {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Close"), argumentList);
    }

  Q_SIGNALS: // SIGNALS
    void Response(uint response, QVariantMap results);
};

namespace org {
namespace freedesktop {
namespace portal {
typedef ::OrgFreedesktopPortalRequestInterface Request;
}
} // namespace freedesktop
} // namespace org
