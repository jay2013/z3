/*++
Copyright (c) 2015 Microsoft Corporation

Module Name:

    rlimit.cpp

Abstract:

    Resource limit container.

Author:

    Nikolaj Bjorner (nbjorner) 2015-09-28

Revision History:

--*/
#include "rlimit.h"

reslimit::reslimit():
    m_cancel(false),
    m_count(0),
    m_limit(0) {
}

uint64 reslimit::count() const {
    return m_count;
}

bool reslimit::inc() {
    ++m_count;
    return !m_cancel && (m_limit == 0 || m_count <= m_limit);
}

bool reslimit::inc(unsigned offset) {
    m_count += offset;
    return !m_cancel && (m_limit == 0 || m_count <= m_limit);
}

void reslimit::push(unsigned delta_limit) {
    uint64 new_limit = delta_limit + m_count;
    if (new_limit <= m_count) {
        new_limit = 0;
    }
    m_limits.push_back(m_limit);
    m_limit = m_limit==0?new_limit:std::min(new_limit, m_limit);
}

void reslimit::pop() {
    if (m_count > m_limit && m_limit > 0) {
        m_count = m_limit;
    }
    m_limit = m_limits.back();
    m_limits.pop_back();
}
