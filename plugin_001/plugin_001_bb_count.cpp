#include <iostream>

/* First gcc header to be included */
#include "gcc-plugin.h"
#include "plugin-version.h"

#include "tree-pass.h"
#include "context.h"
#include "basic-block.h"
#include "gimple-pretty-print.h"

/* Needs to included for plugins to be GPL compatible */
int plugin_is_GPL_compatible;

/* Additional information about the plugin. */
static struct plugin_info my_plugin_info =
{ "Ver 0.1", "This is a test plugin" };

const pass_data my_pass_data =
{
  /* Optimization pass type.*/
  GIMPLE_PASS,
  /* Terse name of the pass used as a fragment of the dump file
  name. If the name starts with a star, no dump happens. */
  "my_first_pass",
  /* The -fopt-info optimization group flags as defined in dumpfile.h. */
  OPTGROUP_NONE,/* Not an optimaization pass */
  /* The timevar id associated with this pass. */
  TV_NONE,
  /* Sets of properties input and output from this pass. */
  PROP_gimple_any, /* entire gimple grammar */
  0,
  0,
  /* Flags indicating common sets things to do before and after. */
  0,
  0
};

/* Number of functions in the code */
unsigned int numb_of_func_count = 0;

class my_first_pass: public gimple_opt_pass
{
    public:
	my_first_pass(gcc::context * ctx):
		gimple_opt_pass(my_pass_data, ctx)
	{ }
	/* Number of Basic blocks in the Function */
	unsigned int numb_of_bb_count = 0;

	virtual unsigned int execute(function * func) override
	{
		basic_block bb;

		FOR_ALL_BB_FN(bb, func)
		{
			/* Index 0 is for ENTRY */
			if(bb->index == 0)
			{
				std::cerr <<"\n Entrying function: " << function_name(func) << "\n";
				numb_of_func_count++;
			}
			/* Index 1 is for EXIT */
			else if(bb->index == 1)
			{
				std::cerr <<"\n Number of Basic Block in the function: "<< numb_of_bb_count << "\n";
				std::cerr <<"\n Exiting function: "<< function_name(func) << "\n";
				numb_of_bb_count = 0;
			}
			else
			{
				numb_of_bb_count++;
				//print_gimple_seq(stderr, bb_info->seq, 0, TDF_NONE);
				//gimple_dump_bb(stderr, bb, 0, TDF_NONE);
			}
		}

		return 0;
	}

	/* we do not clone */
	virtual my_first_pass *clone() override
	{
		return this;
	}
};

void callback_plugin_finish(void *gcc_data, void *numb_of_func_count)
{
	std::cerr <<"\n Number of functions: "<< *(int*)numb_of_func_count << "\n";
}

int plugin_init(struct plugin_name_args *plugin_info,
                struct plugin_gcc_version *ver)
{
  if(!plugin_default_version_check(ver, &gcc_version))
  {
    std::cerr << "Plugin version is: " << GCCPLUGIN_VERSION_MAJOR
              << "." << GCCPLUGIN_VERSION_MINOR << "\n";
    return 1;
  }

  register_callback(plugin_info->base_name, PLUGIN_INFO, NULL, &my_plugin_info);

  /* Register the pass after cfg */
  struct register_pass_info pass_info;

  pass_info.pass = new my_first_pass(g);
  pass_info.reference_pass_name = "cfg";
  pass_info.ref_pass_instance_number = 0x1;
  pass_info.pos_op = PASS_POS_INSERT_AFTER;

  register_callback(plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL,&pass_info);
  register_callback(plugin_info->base_name, PLUGIN_FINISH, callback_plugin_finish, &numb_of_func_count);
  return 0;
}
