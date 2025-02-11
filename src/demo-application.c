/* demo-application.c
 *
 * Copyright 2023 plex
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
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "config.h"

#include "demo-application.h"
#include "demo-window.h"

struct _DemoApplication
{
  GtkApplication parent_instance;
};

G_DEFINE_TYPE (DemoApplication, demo_application, GTK_TYPE_APPLICATION)

DemoApplication *
demo_application_new (const char        *application_id,
                      GApplicationFlags  flags)
{
  g_return_val_if_fail (application_id != NULL, NULL);

  return g_object_new (DEMO_TYPE_APPLICATION,
                       "application-id", application_id,
                       "flags", flags,
                       NULL);
}

static void
demo_application_activate (GApplication *app)
{
  GtkWindow *window;

  g_assert (DEMO_IS_APPLICATION (app));

  window = gtk_application_get_active_window (GTK_APPLICATION (app));
  if (window == NULL)
    window = g_object_new (DEMO_TYPE_WINDOW,
                           "application", app,
                           NULL);

  gtk_window_present (window);
}

static void
demo_application_class_init (DemoApplicationClass *klass)
{
  GApplicationClass *app_class = G_APPLICATION_CLASS (klass);

  app_class->activate = demo_application_activate;
}

static void
demo_application_about_action (GSimpleAction *action,
                               GVariant      *parameter,
                               gpointer       user_data)
{
  static const char *authors[] = {"plex", NULL};
  DemoApplication *self = user_data;
  GtkWindow *window = NULL;

  g_assert (DEMO_IS_APPLICATION (self));

  window = gtk_application_get_active_window (GTK_APPLICATION (self));

  gtk_show_about_dialog (window,
                         "program-name", "demo",
                         "logo-icon-name", "org.gnome.Example",
                         "authors", authors,
                         "version", "0.1.0",
                         "copyright", "© 2023 plex",
                         NULL);
}

static void
demo_application_quit_action (GSimpleAction *action,
                              GVariant      *parameter,
                              gpointer       user_data)
{
  DemoApplication *self = user_data;

  g_assert (DEMO_IS_APPLICATION (self));

  g_application_quit (G_APPLICATION (self));
}

static const GActionEntry app_actions[] = {
  { "quit", demo_application_quit_action },
  { "about", demo_application_about_action },
};

static void
demo_application_init (DemoApplication *self)
{
  g_action_map_add_action_entries (G_ACTION_MAP (self),
                                   app_actions,
                                   G_N_ELEMENTS (app_actions),
                                   self);
  gtk_application_set_accels_for_action (GTK_APPLICATION (self),
                                         "app.quit",
                                         (const char *[]) { "<primary>q", NULL });
}

