
/* Ekiga -- A VoIP and Video-Conferencing application
 * Copyright (C) 2000-2008 Damien Sandras
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 * Ekiga is licensed under the GPL license and as a special exception,
 * you have permission to link or otherwise combine this program with the
 * programs OPAL, OpenH323 and PWLIB, and distribute the combination,
 * without applying the requirements of the GNU GPL to the OPAL, OpenH323
 * and PWLIB programs, as long as you do follow the requirements of the
 * GNU GPL for all the rest of the software thus combined.
 */


/*
 *                         rl-cluster.h  -  description
 *                         ------------------------------------------
 *   begin                : written in 2008 by Julien Puydt
 *   copyright            : (c) 2008 by Julien Puydt
 *   description          : resource-list cluster declaration
 *
 */

#ifndef __RL_CLUSTER_H__
#define __RL_CLUSTER_H__

#include "rl-heap.h"

#include "cluster-impl.h"

namespace RL {

  class Cluster:
    public Ekiga::ClusterImpl<Heap>,
    public Ekiga::Service
  {
  public:

    Cluster (Ekiga::ServiceCore& core_);

    ~Cluster ();

    const std::string get_name () const
    { return "resource-list"; }

    const std::string get_description () const
    { return "Code for support for resource-list"; }

    bool populate_menu (Ekiga::MenuBuilder& builder);

  private:

    Ekiga::ServiceCore& core;
    xmlDocPtr doc;

    void add (xmlNodePtr node);
    void add (const std::string uri,
	      const std::string username,
	      const std::string password,
	      const std::string name);
    void common_add (Heap& heap);
    void save () const;

    void new_heap (const std::string name,
		   const std::string uri,
		   const std::string username,
		   const std::string password);

    void on_new_heap_form_submitted (Ekiga::Form& result);

    void on_presence_received (std::string uri,
			       std::string presence);
    void on_status_received (std::string uri,
			     std::string presence);
  };
};

#endif