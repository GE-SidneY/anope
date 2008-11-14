/* OperServ core functions
 *
 * (C) 2003-2008 Anope Team
 * Contact us at info@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church. 
 * 
 * $Id$
 *
 */
/*************************************************************************/

#include "module.h"

int do_ignorelist(User * u);
void myOperServHelp(User * u);
int do_ignoreuser(User * u);

class OSIgnore : public Module
{
 public:
	OSIgnore(const std::string &modname, const std::string &creator) : Module(modname, creator)
	{
		Command *c;

		this->SetAuthor("Anope");
		this->SetVersion("$Id$");
		this->SetType(CORE);
		c = createCommand("IGNORE", do_ignoreuser, is_services_admin, OPER_HELP_IGNORE, -1, -1, -1, -1);
		this->AddCommand(OPERSERV, c, MOD_UNIQUE);

		moduleSetOperHelp(myOperServHelp);
	}
};


/**
 * Add the help response to anopes /os help output.
 * @param u The user who is requesting help
 **/
void myOperServHelp(User * u)
{
	if (is_services_admin(u)) {
		notice_lang(s_OperServ, u, OPER_HELP_CMD_IGNORE);
	}
}

/**
 * The /os ignore command.
 * @param u The user who issued the command
 * @param MOD_CONT to continue processing other modules, MOD_STOP to stop processing.
 **/
int do_ignoreuser(User * u)
{
	char *cmd = strtok(NULL, " ");
	int t;

	if (!cmd) {
		notice_lang(s_OperServ, u, OPER_IGNORE_SYNTAX);
		return MOD_CONT;
	}

	if (!stricmp(cmd, "ADD")) {
		char *time = strtok(NULL, " ");
		char *nick = strtok(NULL, " ");
		char *rest = strtok(NULL, "");

		if (!nick) {
			notice_lang(s_OperServ, u, OPER_IGNORE_SYNTAX);
			return MOD_CONT;
		} else if (!time) {
			notice_lang(s_OperServ, u, OPER_IGNORE_SYNTAX);
			return MOD_CONT;
		} else {
			t = dotime(time);
			rest = NULL;

			if (t <= -1) {
				notice_lang(s_OperServ, u, OPER_IGNORE_VALID_TIME);
				return MOD_CONT;
			} else if (t == 0) {
				add_ignore(nick, t);
				notice_lang(s_OperServ, u, OPER_IGNORE_PERM_DONE, nick);
			} else {
				add_ignore(nick, t);
				notice_lang(s_OperServ, u, OPER_IGNORE_TIME_DONE, nick, time);
			}
		}
	} else if (!stricmp(cmd, "LIST")) {
		do_ignorelist(u);

	} else if (!stricmp(cmd, "DEL")) {
		char *nick = strtok(NULL, " ");
		if (!nick) {
			notice_lang(s_OperServ, u, OPER_IGNORE_SYNTAX);
		} else {
			if (delete_ignore(nick)) {
				notice_lang(s_OperServ, u, OPER_IGNORE_DEL_DONE, nick);
				return MOD_CONT;
			}
			notice_lang(s_OperServ, u, OPER_IGNORE_LIST_NOMATCH, nick);
		}
	} else if (!stricmp(cmd, "CLEAR")) {
		clear_ignores();
		notice_lang(s_OperServ, u, OPER_IGNORE_LIST_CLEARED);
		return MOD_CONT;
	} else
		notice_lang(s_OperServ, u, OPER_IGNORE_SYNTAX);

	return MOD_CONT;
}

/* shows the Services ignore list */
int do_ignorelist(User * u)
{
	IgnoreData *id;

	if (!ignore) {
		notice_lang(s_OperServ, u, OPER_IGNORE_LIST_EMPTY);
		return MOD_CONT;
	}

	notice_lang(s_OperServ, u, OPER_IGNORE_LIST);
	for (id = ignore; id; id = id->next)
		notice_user(s_OperServ, u, "%s", id->mask);

	return MOD_CONT;
}

MODULE_INIT("os_ignore", OSIgnore)
