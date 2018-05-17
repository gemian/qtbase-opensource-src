/****************************************************************************
**
** Copyright (C) 2013 Klaralvdalens Datakonsult AB (KDAB)
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
**
** This file was generated by glgen version 0.1
** Command line was: glgen
**
** glgen is Copyright (C) 2013 Klaralvdalens Datakonsult AB (KDAB)
**
** This is an auto-generated file.
** Do not edit! All changes made to it will be lost.
**
****************************************************************************/

#include "qopenglversionfunctions.h"
#include "qopenglcontext.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

#define QT_OPENGL_COUNT_FUNCTIONS(ret, name, args) +1
#define QT_OPENGL_FUNCTION_NAMES(ret, name, args) \
    "gl"#name"\0"
#define QT_OPENGL_IMPLEMENT(CLASS, FUNCTIONS) \
void CLASS::init() \
{ \
    const char *names = FUNCTIONS(QT_OPENGL_FUNCTION_NAMES); \
    const char *name = names; \
    for (int i = 0; i < FUNCTIONS(QT_OPENGL_COUNT_FUNCTIONS); ++i) { \
        functions[i] = context->getProcAddress(name); \
        name += strlen(name) + 1; \
    } \
}

QOpenGLVersionFunctionsStorage::QOpenGLVersionFunctionsStorage()
    : backends(0)
{
}

QOpenGLVersionFunctionsStorage::~QOpenGLVersionFunctionsStorage()
{
#ifndef QT_OPENGL_ES
    if (backends) {

        int i = 0;

#define DELETE_BACKEND(X) \
        if (backends[i] && !--backends[i]->refs) \
            delete static_cast<QOpenGLFunctions_##X##Backend*>(backends[i]); \
        ++i;

        QT_OPENGL_VERSIONS(DELETE_BACKEND)
#undef DELETE_BACKEND
        delete[] backends;
    }
#endif
}

QOpenGLVersionFunctionsBackend *QOpenGLVersionFunctionsStorage::backend(QOpenGLContext *context, QOpenGLVersionFunctionsBackend::Version v)
{
#ifdef QT_OPENGL_ES
    Q_UNUSED(context);
    Q_UNUSED(v);
    return 0;
#else
    if (!backends) {
        backends = new QOpenGLVersionFunctionsBackend *[QOpenGLVersionFunctionsBackend::OpenGLVersionBackendCount];
        memset(backends, 0, sizeof(QOpenGLVersionFunctionsBackend *)*QOpenGLVersionFunctionsBackend::OpenGLVersionBackendCount);
    }
    if (backends[v])
        return backends[v];

    switch(v) {
#define VERSION_ENUM(X) QOpenGLVersionFunctionsBackend::OpenGL_##X
#define CREATE_BACKEND(X) \
    case VERSION_ENUM(X): \
        backends[VERSION_ENUM(X)] = new QOpenGLFunctions_##X##Backend(context); \
        break;
    QT_OPENGL_VERSIONS(CREATE_BACKEND)
    case QOpenGLVersionFunctionsBackend::OpenGLVersionBackendCount:
        Q_UNREACHABLE();
    }
    // the storage keeps one ref
    ++backends[v]->refs;
    return backends[v];
#endif
}

QOpenGLVersionFunctionsBackend *QAbstractOpenGLFunctionsPrivate::functionsBackend(QOpenGLContext *context, QOpenGLVersionFunctionsBackend::Version v)
{
    Q_ASSERT(context);
    QOpenGLVersionFunctionsStorage *storage = context->functionsBackendStorage();
    return storage->backend(context, v);
}

void QAbstractOpenGLFunctionsPrivate::insertExternalFunctions(QOpenGLContext *context, QAbstractOpenGLFunctions *f)
{
    Q_ASSERT(context);
    context->insertExternalFunctions(f);
}

void QAbstractOpenGLFunctionsPrivate::removeExternalFunctions(QOpenGLContext *context, QAbstractOpenGLFunctions *f)
{
    Q_ASSERT(context);
    context->removeExternalFunctions(f);
}

/*!
    \class QAbstractOpenGLFunctions
    \inmodule QtGui
    \since 5.1
    \brief The QAbstractOpenGLFunctions class is the base class of a family of
           classes that expose all functions for each OpenGL version and
           profile.

    OpenGL implementations on different platforms are able to link to a variable
    number of OpenGL functions depending upon the OpenGL ABI on that platform.
    For example, on Microsoft Windows only functions up to those in OpenGL 1.1
    can be linked to at build time. All other functions must be resolved at
    runtime. The traditional solution to this has been to use either
    QOpenGLContext::getProcAddress() or QOpenGLFunctions. The former is tedious
    and error prone and means dealing directly with function pointers. The
    latter only exposes those functions common to OpenGL ES 2 and desktop
    OpenGL. There is however much new OpenGL functionality that is useful when
    writing real world OpenGL applications.

    Qt now provides a family of classes which all inherit from
    QAbstractOpenGLFunctions which expose every core OpenGL function by way of a
    corresponding member function. There is a class for every valid combination
    of OpenGL version and profile. Each class follows the naming convention:
    \badcode
    QOpenGLFunctions_<MAJOR VERSION>_<MINOR VERSION>[_PROFILE]
    \endcode

    For OpenGL versions 1.0 through to 3.0 there are no profiles, leading to the
    classes:

    \list
        \li QOpenGLFunctions_1_0
        \li QOpenGLFunctions_1_1
        \li QOpenGLFunctions_1_2
        \li QOpenGLFunctions_1_3
        \li QOpenGLFunctions_1_4
        \li QOpenGLFunctions_1_5
        \li QOpenGLFunctions_2_0
        \li QOpenGLFunctions_2_1
        \li QOpenGLFunctions_3_0
    \endlist

    where each class inherits from QAbstractOpenGLFunctions.

    OpenGL version 3.1 removed many deprecated functions leading to a much
    simpler and generic API.

    With OpenGL 3.2 the concept of profiles was introduced. Two profiles are
    currently defined for OpenGL: Core and Compatibility.

    The Core profile does not include any of the functions that were removed
    in OpenGL 3.1. The Compatibility profile contains all functions in the
    Core profile of the same version plus all of the functions that were
    removed in OpenGL 3.1. In this way the Compatibility profile classes allow
    use of newer OpenGL functionality but also allows you to keep using your
    legacy OpenGL code. For new OpenGL code the Core profile should be
    preferred.

    Please note that some vendors, notably Apple, do not implement the
    Compatibility profile. Therefore if you wish to target new OpenGL features
    on \macos then you should ensure that you request a Core profile context via
    QSurfaceFormat::setProfile().

    Qt provides classes for all version and Core and Compatibility profile
    combinations. The classes for OpenGL versions 3.1 through to 4.3 are:

    \list
        \li QOpenGLFunctions_3_1
        \li QOpenGLFunctions_3_2_Core
        \li QOpenGLFunctions_3_2_Compatibility
        \li QOpenGLFunctions_3_3_Core
        \li QOpenGLFunctions_3_3_Compatibility
        \li QOpenGLFunctions_4_0_Core
        \li QOpenGLFunctions_4_0_Compatibility
        \li QOpenGLFunctions_4_1_Core
        \li QOpenGLFunctions_4_1_Compatibility
        \li QOpenGLFunctions_4_2_Core
        \li QOpenGLFunctions_4_2_Compatibility
        \li QOpenGLFunctions_4_3_Core
        \li QOpenGLFunctions_4_3_Compatibility
    \endlist

    where each class inherits from QAbstractOpenGLFunctions.

    A pointer to an object of the class corresponding to the version and
    profile of OpenGL in use can be obtained from
    QOpenGLContext::versionFunctions(). If obtained in this way, note that
    the QOpenGLContext retains ownership of the object. This is so that only
    one instance need be created.

    Before calling any of the exposed OpenGL functions you must ensure that the
    object has resolved the function pointers to the OpenGL functions. This
    only needs to be done once per instance with initializeOpenGLFunctions().
    Once initialized, the object can be used to call any OpenGL function for
    the corresponding version and profile. Note that initializeOpenGLFunctions()
    can fail in some circumstances so check the return value. Situations in
    which initialization can fail are if you have a functions object for a version
    or profile that contains functions that are not part of the context being
    used to resolve the function pointers.

    If you exclusively use function objects then you will get compile time
    errors if you attempt to use a function not included in that version and
    profile. This is obviously a lot easier to debug than undefined behavior
    at run time.

    \sa QOpenGLContext::versionFunctions()
*/
/*!
   Constructs a QAbstractOpenGLFunctions object.
*/
QAbstractOpenGLFunctions::QAbstractOpenGLFunctions()
    : d_ptr(new QAbstractOpenGLFunctionsPrivate)
{
}

/*!
   Destroys a QAbstractOpenGLFunctions object.
*/
QAbstractOpenGLFunctions::~QAbstractOpenGLFunctions()
{
    Q_D(QAbstractOpenGLFunctions);
    if (d->owningContext)
        d->removeExternalFunctions(d->owningContext, this);
    delete d_ptr;
}

/*! \internal
 */
bool QAbstractOpenGLFunctions::initializeOpenGLFunctions()
{
    Q_D(QAbstractOpenGLFunctions);
    d->initialized = true;

    // For a subclass whose instance is not created via
    // QOpenGLContext::versionFunctions() owningContext is not set. Set it now
    // and register such instances to the context as external ones. These are
    // not owned by the context but still need certain cleanup when the context
    // is destroyed.
    if (!d->owningContext) {
        d->owningContext = QOpenGLContext::currentContext();
        if (d->owningContext)
            d->insertExternalFunctions(d->owningContext, this);
    }

    return true;
}

/*! \internal
 */
bool QAbstractOpenGLFunctions::isInitialized() const
{
    Q_D(const QAbstractOpenGLFunctions);
    return d->initialized;
}

/*! \internal
 */
void QAbstractOpenGLFunctions::setOwningContext(const QOpenGLContext *context)
{
    Q_D(QAbstractOpenGLFunctions);
    d->owningContext = const_cast<QOpenGLContext*>(context);
}

/*! \internal
 */
QOpenGLContext *QAbstractOpenGLFunctions::owningContext() const
{
    Q_D(const QAbstractOpenGLFunctions);
    return d->owningContext;
}

#if !defined(QT_OPENGL_ES_2)

QT_OPENGL_IMPLEMENT(QOpenGLFunctions_1_0_CoreBackend, QT_OPENGL_1_0_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_1_1_CoreBackend, QT_OPENGL_1_1_FUNCTIONS)

QT_OPENGL_IMPLEMENT(QOpenGLFunctions_1_2_CoreBackend, QT_OPENGL_1_2_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_1_3_CoreBackend, QT_OPENGL_1_3_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_1_4_CoreBackend, QT_OPENGL_1_4_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_1_5_CoreBackend, QT_OPENGL_1_5_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_2_0_CoreBackend, QT_OPENGL_2_0_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_2_1_CoreBackend, QT_OPENGL_2_1_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_3_0_CoreBackend, QT_OPENGL_3_0_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_3_1_CoreBackend, QT_OPENGL_3_1_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_3_2_CoreBackend, QT_OPENGL_3_2_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_3_3_CoreBackend, QT_OPENGL_3_3_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_4_0_CoreBackend, QT_OPENGL_4_0_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_4_1_CoreBackend, QT_OPENGL_4_1_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_4_2_CoreBackend, QT_OPENGL_4_2_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_4_3_CoreBackend, QT_OPENGL_4_3_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_4_4_CoreBackend, QT_OPENGL_4_4_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_4_5_CoreBackend, QT_OPENGL_4_5_FUNCTIONS)

QT_OPENGL_IMPLEMENT(QOpenGLFunctions_1_0_DeprecatedBackend, QT_OPENGL_1_0_DEPRECATED_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_1_1_DeprecatedBackend, QT_OPENGL_1_1_DEPRECATED_FUNCTIONS)

QT_OPENGL_IMPLEMENT(QOpenGLFunctions_1_2_DeprecatedBackend, QT_OPENGL_1_2_DEPRECATED_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_1_3_DeprecatedBackend, QT_OPENGL_1_3_DEPRECATED_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_1_4_DeprecatedBackend, QT_OPENGL_1_4_DEPRECATED_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_2_0_DeprecatedBackend, QT_OPENGL_2_0_DEPRECATED_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_3_0_DeprecatedBackend, QT_OPENGL_3_0_DEPRECATED_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_3_3_DeprecatedBackend, QT_OPENGL_3_3_DEPRECATED_FUNCTIONS)
QT_OPENGL_IMPLEMENT(QOpenGLFunctions_4_5_DeprecatedBackend, QT_OPENGL_4_5_DEPRECATED_FUNCTIONS)

#else

// No backends for OpenGL ES 2

#endif // !QT_OPENGL_ES_2

QT_END_NAMESPACE
