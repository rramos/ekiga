
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
 *                         rlist-cluster.cpp  -  description
 *                         ------------------------------------------
 *   begin                : written in 2008 by Julien Puydt
 *   copyright            : (c) 2008 by Julien Puydt
 *   description          : resource-list cluster implementation
 *
 */

#include "config.h"

#include "rl-cluster.h"

#include "gmconf.h"
#include "form-request-simple.h"
#include "presence-core.h"

#include <iostream>

#define KEY "/apps/" PACKAGE_NAME "/contacts/resource-lists"

RL::Cluster::Cluster (Ekiga::ServiceCore& core_): core(core_), doc(NULL)
{
  gchar* c_raw = NULL;

  gmref_ptr<Ekiga::PresenceCore> presence_core = core.get ("presence-core");

  presence_core->presence_received.connect (sigc::mem_fun (this, &RL::Cluster::on_presence_received));
  presence_core->status_received.connect (sigc::mem_fun (this, &RL::Cluster::on_status_received));

  c_raw = gm_conf_get_string (KEY);

  if (c_raw != NULL) {

    const std::string raw = c_raw;
    doc = xmlRecoverMemory (raw.c_str (), raw.length ());

    xmlNodePtr root = xmlDocGetRootElement (doc);
    if (root == NULL) {

      root = xmlNewDocNode (doc, NULL, BAD_CAST "list", NULL);
      xmlDocSetRootElement (doc, root);
    } else {

      for (xmlNodePtr child = root->children;
	   child != NULL;
	   child = child->next)
	if (child->type == XML_ELEMENT_NODE
	    && child->name != NULL
	    && xmlStrEqual (BAD_CAST "entry", child->name))
	  add (child);
    }
    g_free (c_raw);

  } else {

    doc = xmlNewDoc (BAD_CAST "1.0");
    xmlNodePtr root = xmlNewDocNode (doc, NULL, BAD_CAST "list", NULL);
    xmlDocSetRootElement (doc, root);
    add ("http://localhost:443", "", "", "test@ekiga.net", "XCAP Test"); // FIXME: remove
  }
}

RL::Cluster::~Cluster ()
{
  if (doc != NULL)
    xmlFreeDoc (doc);
}

bool
RL::Cluster::populate_menu (Ekiga::MenuBuilder& builder)
{
  builder.add_action ("new", _("New resource list"),
		      sigc::bind (sigc::mem_fun (this, &RL::Cluster::new_heap),
				  "", "", "", "", ""));
  return true;
}

void
RL::Cluster::add (xmlNodePtr node)
{
  Heap* heap = new Heap (core, node);

  common_add (*heap);
}

void
RL::Cluster::add (const std::string uri,
		  const std::string username,
		  const std::string password,
		  const std::string user,
		  const std::string name)
{
  Heap* heap = new Heap (core, name, uri, username, password, user);
  xmlNodePtr root = xmlDocGetRootElement (doc);

  xmlAddChild (root, heap->get_node ());

  save ();
  common_add (*heap);
}

void
RL::Cluster::common_add (Heap& heap)
{
  add_heap (heap);

  // FIXME: here we should ask for presence for the heap...

  heap.trigger_saving.connect (sigc::mem_fun (this, &RL::Cluster::save));
}

void
RL::Cluster::save () const
{
  xmlChar* buffer = NULL;
  int size = 0;

  xmlDocDumpMemory (doc, &buffer, &size);

  gm_conf_set_string (KEY, (const char*)buffer);

  xmlFree (buffer);
}

void
RL::Cluster::new_heap (const std::string name,
		       const std::string uri,
		       const std::string username,
		       const std::string password,
		       const std::string user)
{
  Ekiga::FormRequestSimple request;

  request.title (_("Add new resource-list"));
  request.instructions (_("Please fill in this form to add a new "
			  "contact list to ekiga's remote roster"));
  request.text ("name", _("Name:"), name);
  request.text ("uri", _("Address:"), uri);
  request.text ("username", _("Username:"), username);
  request.private_text ("password", _("Password:"), password);
  request.text ("user", _("User:"), user);

  request.submitted.connect (sigc::mem_fun (this, &RL::Cluster::on_new_heap_form_submitted));

  if (!questions.handle_request (&request)) {

    // FIXME: better error reporting
#ifdef __GNUC__
    std::cout << "Unhandled form request in "
	      << __PRETTY_FUNCTION__ << std::endl;
#endif
  }
}

void
RL::Cluster::on_new_heap_form_submitted (Ekiga::Form& result)
{
  try {

    const std::string name = result.text ("name");
    const std::string uri = result.text ("uri");
    const std::string username = result.text ("username");
    const std::string password = result.private_text ("password");
    const std::string user = result.text ("user");

    add (name, uri, username, password, user);
  } catch (Ekiga::Form::not_found) {

#ifdef __GNUC__
    std::cerr << "Invalid form submitted to "
	      << __PRETTY_FUNCTION__ << std::endl;
#endif
  }
}


void
RL::Cluster::on_presence_received (std::string uri,
				   std::string presence)
{
  for (iterator iter = begin ();
       iter != end ();
       ++iter) {

    iter->push_presence (uri, presence);
  }
}

void
RL::Cluster::on_status_received (std::string uri,
				 std::string status)
{
  for (iterator iter = begin ();
       iter != end ();
       ++iter) {

    iter->push_status (uri, status);
  }
}