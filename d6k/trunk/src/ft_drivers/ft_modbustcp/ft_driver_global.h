#ifndef FT_MODBUSTCP_GLOBAL_H
#define FT_MODBUSTCP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FT_MODBUSTCP_LIBRARY)
#  define FT_MODBUSTCPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FT_MODBUSTCPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FT_MODBUSTCP_GLOBAL_H
