/* MemoServ core functions
 *
 * (C) 2003-2010 Anope Team
 * Contact us at team@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church.
 */

/*************************************************************************/

#include "module.h"

class CommandMSHelp : public Command
{
 public:
	CommandMSHelp() : Command("HELP", 1, 1)
	{
		this->SetFlag(CFLAG_ALLOW_UNREGISTERED);
	}

	CommandReturn Execute(User *u, const std::vector<Anope::string> &params)
	{
		mod_help_cmd(MemoServ, u, params[0]);
		return MOD_CONT;
	}

	void OnSyntaxError(User *u, const Anope::string &subcommand)
	{
		notice_help(Config.s_MemoServ, u, MEMO_HELP_HEADER);
		for (CommandMap::const_iterator it = MemoServ->Commands.begin(), it_end = MemoServ->Commands.end(); it != it_end; ++it)
			if (!Config.HidePrivilegedCommands || it->second->permission.empty() || (u->Account() && u->Account()->HasCommand(it->second->permission)))
				it->second->OnServHelp(u);
		notice_help(Config.s_MemoServ, u, MEMO_HELP_FOOTER, Config.s_ChanServ.c_str());
	}
};

class MSHelp : public Module
{
 public:
	MSHelp(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator)
	{
		this->SetAuthor("Anope");
		this->SetType(CORE);

		this->AddCommand(MemoServ, new CommandMSHelp());
	}
};

MODULE_INIT(MSHelp)