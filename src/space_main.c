/* space_main.c */

#include "config.h"
#include "space.h"

/* ------------------------------------------------------------------------ */

struct pennmush_flag_info
{
	const char* name;
	char letter;
	int type;
	int perms;
	int negate_perms;
};

struct pennmush_flag_info aspace_power_table[] =
{
     {"SDB-READ", '\0', NOTYPE, F_WIZARD | F_LOG, F_WIZARD},
     {"SDB-OK", '\0', NOTYPE, F_WIZARD | F_LOG, F_WIZARD},
     {NULL,'0',0,0,0}
};

struct pennmush_flag_info aspace_flag_table[] = 
{
	{"SPACE-OBJECT", '+', TYPE_THING | TYPE_PLAYER, F_WIZARD, F_WIZARD},
	{"PLANET", 'P', TYPE_THING, F_WIZARD, F_WIZARD},
	{"NEBULA", 'N', TYPE_THING, F_WIZARD, F_WIZARD},
	{"ANOMALY", 'Y', TYPE_THING | TYPE_PLAYER, F_WIZARD, F_WIZARD},
	{"BASE", 'B', TYPE_THING, F_WIZARD, F_WIZARD},
	{"SHIP", 'p', TYPE_THING, F_WIZARD, F_WIZARD},
	{NULL,'0',0,0,0}
};

/* border stuff */


/* ------------------------------------------------------------------------ */

// Jarek recoded along with spacenum to return nice lists of possible matchs
void lookup_space (const char *name, dbref *space)
{
	dbref thing;
	int i = 0;

	space[0] = NOTHING;

	if (is_dbref(name)) {
		space[0] = parse_dbref(name);
		if (!(SpaceObj(space[0]) && GoodObject(space[0])))
			space[0] = NOTHING;
	} else
		for (thing = 0; thing < db_top; thing++)
			if (SpaceObj(thing) && GoodObject(thing))
				if (local_wild_match(name, Name(thing)))
				{
					space[i] = thing;
      					i++;
				}

	space[i] = NOTHING;
}

/* ------------------------------------------------------------------------ */

return_t
  convert_double (char *input, double deflt, double *output)
{
  if (is_number(input)) {
    *output = atof(input);
    return SUCCESS;
  } else {
    *output = deflt;
    return CONVERSION_ERROR;
  }
}

/* ------------------------------------------------------------------------ */

return_t
  convert_long (char *input, long deflt, int *output)
{
   if (input==NULL) {
    *output = deflt;
    return CONVERSION_ERROR;;
   } else if (is_number(input)) {
    *output = atoi(input);
    return SUCCESS;
  } else {
    *output = deflt;
    return CONVERSION_ERROR;
  }
}

/* ------------------------------------------------------------------------ */

/* ARGSUSED */
FUNCTION(local_fun_sdb) /* sdb (<function>[,<field>[,<field>[,<field>[,<field>[,...]]]]]) */
{
	dbref ship, console;
	int s, x;
	double a;
	static char blank[] = "   \0";

	if (!Wizard(executor) && !has_power_by_name(executor, "SDB-OK", NOTYPE) && !has_power_by_name(executor, "SDB-READ", NOTYPE)) {
		/* Mordak Aspace v1.0.0p1 Removed Cursing from a major function call*/
		safe_str("#-1 Permission Denied", buff, bp);
		/* End Aspace v1.0.0p1 */
		return;
	}

	for (x = 0; x < 9; ++x)
		if (args[x] == NULL)
			args[x] = blank;
	switch (args[0][0]) {
		case 'd': /* do */
			n = parse_integer(args[1]);
			if (!GoodSDB(n)) {
				safe_str("#-1 SDB OUT OF RANGE", buff, bp); return; }
			switch (parse_integer(args[2])) {
				case  0: safe_str(unparse_integer(debug_space(n)), buff, bp); break;
				case  1: safe_str(unparse_integer(do_set_main_reactor(parse_number(args[3]), enactor)), buff, bp); break;
				case  2: safe_str(unparse_integer(do_set_aux_reactor(parse_number(args[3]), enactor)), buff, bp); break;
				case  3: safe_str(unparse_integer(do_set_battery(parse_number(args[3]), enactor)), buff, bp); break;
				case  4: safe_str(unparse_integer(do_set_course(parse_number(args[3]), parse_number(args[4]), parse_number(args[5]), enactor)), buff, bp); break;
				case  5: safe_str(unparse_integer(do_set_speed(parse_number(args[3]), enactor)), buff, bp); break;
				case  6: safe_str(unparse_integer(do_set_eng_alloc(parse_number(args[3]), parse_number(args[4]), parse_number(args[5]), enactor)), buff, bp); break;
				case  7: safe_str(unparse_integer(do_set_helm_alloc(parse_number(args[3]), parse_number(args[4]), parse_number(args[5]), enactor)), buff, bp); break;
				case  8: safe_str(unparse_integer(do_set_shield_alloc(parse_number(args[3]), parse_number(args[4]), parse_number(args[5]), parse_number(args[6]), parse_number(args[7]), parse_number(args[8]), enactor)), buff, bp); break;
				case  9: safe_str(unparse_integer(do_set_tactical_alloc(parse_number(args[3]), parse_number(args[4]), parse_number(args[5]), enactor)), buff, bp); break;
				case 10: safe_str(unparse_integer(do_set_sensor_alloc(parse_number(args[3]), parse_number(args[4]), enactor)), buff, bp); break;
				case 11: safe_str(unparse_integer(do_set_operations_alloc(parse_number(args[3]), parse_number(args[4]), parse_number(args[5]), enactor)), buff, bp); break;
				case 12: safe_str(unparse_integer(do_set_active(enactor)), buff, bp); break;
				case 13: safe_str(unparse_integer(do_set_inactive(enactor)), buff, bp); break;
				case 14: safe_str(unparse_integer(do_set_dock(parse_integer(args[3]), enactor)), buff, bp); break;
				case 15: safe_str(unparse_integer(do_set_undock(enactor)), buff, bp); break;
				case 16: safe_str(unparse_integer(do_set_shield(parse_integer(args[3]), parse_integer(args[4]), enactor)), buff, bp); break;
				case 17: safe_str(unparse_integer(do_set_trans(parse_integer(args[3]), enactor)), buff, bp); break;
				case 18: safe_str(unparse_integer(do_set_weapon(parse_integer(args[3]), parse_integer(args[4]), parse_integer(args[5]), parse_integer(args[6]), enactor)), buff, bp); break;
				case 19: safe_str(unparse_integer(do_set_trans_locked(parse_integer(args[3]), parse_integer(args[4]), enactor)), buff, bp); break;
				case 20: safe_str(unparse_integer(do_set_autopilot(parse_integer(args[3]), enactor)), buff, bp); break;
				case 21: safe_str(unparse_integer(do_set_trans_unlocked(enactor)), buff, bp); break;
				case 22: safe_str(unparse_integer(do_lock_weapon(parse_integer(args[3]), parse_integer(args[4]), parse_integer(args[5]), parse_integer(args[6]), enactor)), buff, bp); break;
				case 23: safe_str(unparse_integer(do_unlock_weapon(parse_integer(args[3]), parse_integer(args[4]), parse_integer(args[5]), enactor)), buff, bp); break;
				case 24: safe_str(unparse_integer(do_set_docking_doors(parse_integer(args[3]), enactor)), buff, bp); break;
				case 25: safe_str(unparse_integer(do_set_landing_doors(parse_integer(args[3]), enactor)), buff, bp); break;
				case 26: safe_str(unparse_integer(do_set_lrs(parse_integer(args[3]), enactor)), buff, bp); break;
				case 27: safe_str(unparse_integer(do_set_ftr_alloc(parse_number(args[3]), parse_number(args[4]), parse_number(args[5]), enactor)), buff, bp); break;
				case 28: safe_str(unparse_integer(do_set_evade(parse_integer(args[3]), enactor)), buff, bp); break;
				case 29: safe_str(unparse_integer(do_set_srs(parse_integer(args[3]), enactor)), buff, bp); break;
				case 30: safe_str(unparse_integer(do_set_ew(parse_integer(args[3]), enactor)), buff, bp); break;
				case 31: safe_str(unparse_integer(do_set_tract(parse_integer(args[3]), enactor)), buff, bp); break;
				case 32: safe_str(unparse_integer(do_set_cloak(parse_integer(args[3]), enactor)), buff, bp); break;
				case 33: safe_str(unparse_integer(do_set_eta(parse_number(args[3]), enactor)), buff, bp); break;
				case 34: safe_str(unparse_integer(do_set_land(parse_integer(args[3]), enactor)), buff, bp); break;
				case 35: safe_str(unparse_integer(do_set_launch(enactor)), buff, bp); break;
				case 36: safe_str(unparse_integer(do_set_enter_wormhole(parse_integer(args[3]), enactor)), buff, bp); break;
				case 37: safe_str(unparse_integer(do_set_coords_manual(parse_number(args[3]), parse_number(args[4]), parse_number(args[5]), enactor)), buff, bp); break;
				case 38: safe_str(unparse_integer(do_set_coords_reset(enactor)), buff, bp); break;
				case 39: safe_str(unparse_integer(do_set_fix_damage(args[3], args[4], parse_integer(args[5]), args[6], enactor)), buff, bp); break;
				case 40: safe_str(unparse_integer(do_set_intercept(parse_integer(args[3]), enactor)), buff, bp); break;
				case 41: safe_str(unparse_integer(do_set_coords_layin(parse_number(args[3]), parse_number(args[4]), parse_number(args[5]), enactor)), buff, bp); break;
				case 42: safe_str(unparse_integer(do_set_shield_freq(parse_number(args[3]), enactor)), buff, bp); break;
				case 43: safe_str(unparse_integer(do_set_cloak_freq(parse_number(args[3]), enactor)), buff, bp); break;
				case 44: safe_str(unparse_integer(do_set_beam_freq(parse_number(args[3]), enactor)), buff, bp); break;
				case 45: safe_str(unparse_integer(do_set_missile_freq(parse_number(args[3]), enactor)), buff, bp); break;
				case 46: safe_str(unparse_integer(do_set_trans_freq(parse_number(args[3]), enactor)), buff, bp); break;
				case 47: safe_str(unparse_integer(do_set_tract_freq(parse_number(args[3]), enactor)), buff, bp); break;
				case 48: safe_str(unparse_integer(do_set_tract_locked(parse_integer(args[3]), enactor)), buff, bp); break;
				case 49: safe_str(unparse_integer(do_set_tract_unlocked(enactor)), buff, bp); break;
				case 50: safe_str(unparse_integer(do_set_refuel(args[3], args[4], parse_number(args[5]), enactor)), buff, bp); break;
				case 51: safe_str(unparse_integer(do_set_defuel(args[3], parse_number(args[4]), enactor)), buff, bp); break;
				case 52: safe_str(unparse_integer(do_set_fire(parse_integer(args[3]), parse_integer(args[4]), parse_integer(args[5]), parse_integer(args[6]), enactor)), buff, bp); break;
				case 53: safe_str(unparse_integer(do_set_yaw(parse_number(args[3]), enactor)), buff, bp); break;
				case 54: safe_str(unparse_integer(do_set_pitch(parse_number(args[3]), enactor)), buff, bp); break;
				case 55: safe_str(unparse_integer(do_set_roll(parse_number(args[3]), enactor)), buff, bp); break;
				case 56: safe_str(unparse_integer(do_set_coords_engage(enactor)), buff, bp); break;
	
				case 58: safe_str(unparse_integer(do_set_parallel(parse_integer(args[3]), enactor)), buff, bp); break;
				default: safe_str("#-1 NO SUCH FIELD SELECTION", buff, bp); break;
			} break;
		case 'e': /* empty */
			x = get_empty_sdb();
			if (x == VACANCY_FAIL) {
				safe_str("#-1 NO EMPTY SDB NUMBER", buff, bp); return; }
			safe_str(unparse_integer(x), buff, bp); break;
		case 'f': /* function */
			switch (args[2][0]) {
				case 'a': /* angular velocity */
					n = parse_integer(args[1]);
					if (!GoodSDB(n)) {
						safe_str("#-1 SDB 1 OUT OF RANGE", buff, bp); return; }
					x = parse_integer(args[3]);
					if (!GoodSDB(x)) {
						safe_str("#-1 SDB 2 OUT OF RANGE", buff, bp); return; }
					safe_str(unparse_number(sdb2angular(n, x)), buff, bp); break;
				case 'b': /* bearing */
					n = parse_integer(args[1]);
					if (!GoodSDB(n)) {
						safe_str("#-1 SDB 1 OUT OF RANGE", buff, bp); return; }
					x = parse_integer(args[3]);
					if (!GoodSDB(x)) {
						safe_str("#-1 SDB 2 OUT OF RANGE", buff, bp); return; }
					safe_str(unparse_number(sdb2bearing(n, x)), buff, bp);
					safe_chr(' ', buff, bp);
					safe_str(unparse_number(sdb2elevation(n, x)), buff, bp); break;
				case 'd': /* dump */
					(void) dump_space(executor);
					safe_chr('1', buff, bp); break;
				case 'f': /* fix */
					n = parse_integer(args[1]);
					if (!GoodSDB(n)) {
						safe_str("#-1 SDB OUT OF RANGE", buff, bp); return; }
					safe_str(unparse_integer(repair_everything()), buff, bp); break;
				case 'r': /* range */
					n = parse_integer(args[1]);
					if (!GoodSDB(n)) {
						safe_str("#-1 SDB 1 OUT OF RANGE", buff, bp); return; }
					x = parse_integer(args[3]);
					if (!GoodSDB(x)) {
						safe_str("#-1 SDB 2 OUT OF RANGE", buff, bp); return; }
					safe_str(unparse_range(sdb2range(n, x)), buff, bp); break;
				case 's': /* shield */
					n = parse_integer(args[1]);
					if (!GoodSDB(n)) {
						safe_str("#-1 SDB 1 OUT OF RANGE", buff, bp); return; }
					x = parse_integer(args[3]);
					if (!GoodSDB(x)) {
						safe_str("#-1 SDB 2 OUT OF RANGE", buff, bp); return; }
					s = sdb2shield(n, x);
					if (sdb[n].shield.exist && sdb2dissipation(n, s)) {
						safe_str(tprintf("%s Up", unparse_shield(s)), buff, bp); return; }
					safe_str(tprintf("%s Dn", unparse_shield(s)), buff, bp); break;
				case 't': /* transporter */
					n = parse_integer(args[1]);
					if (!GoodSDB(n)) {
						safe_str("#-2 SDB 1 OUT OF RANGE", buff, bp); return; }
					if (!sdb[n].trans.exist) {
						safe_str("#-3 TRANSPORTER NONEXISTANT", buff, bp); return; }
					if (!sdb[n].trans.damage) {
						safe_str("#-4 TRANSPORTER DAMAGED", buff, bp); return; }
					if (!sdb[n].trans.active) {
						safe_str("#-5 TRANSPORTER OFFLINE", buff, bp); return; }
					s = sdb[n].trans.s_lock;
					if (!GoodSDB(s)) {
						safe_str("#-6 TRANSPORTER SOURCE UNLOCKED", buff, bp); return; }
					x = sdb[n].trans.d_lock;
					if (!GoodSDB(x)) {
						safe_str("#-7 TRANSPORTER DEST UNLOCKED", buff, bp); return; }
					if (n != s)
						if (sdb[n].slist.lev[sdb2slist(n, s)] < 0.5) {
							safe_str("#-8 INSUFFICIENT SOURCE RESOLUTION", buff, bp); return; }
					if (n != x)
						if (sdb[n].slist.lev[sdb2slist(n, x)] < 0.5) {
							safe_str("#-9 INSUFFICIENT DEST RESOLUTION", buff, bp); return; }
					if (n != s)
						if (sdb2range(n, s) > MAX_TRANSPORTER_DISTANCE * sdb[n].tech.sensors) {
							safe_str("#-10 SOURCE OUT OF RANGE", buff, bp); return; }
					if (n != x)
						if (sdb2range(n, x) > MAX_TRANSPORTER_DISTANCE * sdb[n].tech.sensors) {
							safe_str("#-11 DEST OUT OF RANGE", buff, bp); return; }
					if (sdb[n].power.total * sdb[n].alloc.transporters < sdb[n].trans.cost) {
						safe_str("#-12 INSUFFICIENT POWER", buff, bp); return; }
					if (s != n) {
						if (sdb[n].shield.exist)
							if (fabs(sdb[n].shield.freq - sdb[n].trans.freq) > 0.005)
								if (sdb2dissipation(n, sdb2shield(n, s)) != 0.0) {
									safe_str("#-13 SOURCE FACING SHIELD UP", buff, bp); return; }
						if (sdb[s].shield.exist)
							if (fabs(sdb[s].shield.freq - sdb[n].trans.freq) > 0.005)
								if (sdb2dissipation(s, sdb2shield(s, n)) != 0.0) {
									safe_str("#-14 SOURCE CONTACT SHIELD UP", buff, bp); return; }
					}
					if (x != n) {
						if (sdb[n].shield.exist)
							if (fabs(sdb[n].shield.freq - sdb[n].trans.freq) > 0.005)
								if (sdb2dissipation(n, sdb2shield(n, x)) != 0.0) {
									safe_str("#-15 DEST FACING SHIELD UP", buff, bp); return; }
						if (sdb[x].shield.exist)
							if (fabs(sdb[n].shield.freq - sdb[x].trans.freq) > 0.005)
								if (sdb2dissipation(x, sdb2shield(x, n)) != 0.0) {
									safe_str("#-16 DEST CONTACT SHIELD UP", buff, bp); return; }
					}
					safe_str(tprintf("%s %s %s", unparse_dbref(sdb[n].object),
					  unparse_dbref(sdb[s].object), unparse_dbref(sdb[x].object)), buff, bp); break;
				default: safe_str("#-1 NO SUCH SDB FUNCTION", buff, bp); break;
			} break;
		case 'g': /* get */
			x = parse_integer(args[1]);
			if (!GoodSDB(x)) {
				safe_str("#-1 SDB OUT OF RANGE", buff, bp); return; }
			do_space_db_get(x, args[2], args[3], args[4], args[5], buff, bp); break;
		case 'i': /* iterate */
			safe_str(unparse_integer(do_space_db_iterate()), buff, bp); break;
		case 'p': /* put */
			x = parse_integer(args[1]);
			if (!GoodSDB(x)) {
				safe_str("#-1 SDB OUT OF RANGE", buff, bp); return; }
			do_space_db_put(x, args[2], args[3], args[4], args[5], args[6], buff, bp); break;
		case 'r': /* read */
	                if (!Wizard(executor) && !has_power_by_name(executor, "SDB-READ", NOTYPE))
	                {
			   safe_str("#-1 Permission Denied", buff, bp);
			}
			ship = parse_dbref(args[1]);
			if (!SpaceObj(ship) || !GoodObject(ship)) {
				safe_str("#-1 INVALID SPACE OBJECT", buff, bp); return; }
			safe_str(unparse_integer(do_space_db_read(ship, executor)), buff, bp); break;
		case 's': /* status */
			n = parse_integer(args[1]);
			if (!GoodSDB(n)) {
				safe_str("#-1 SDB OUT OF RANGE", buff, bp); return; }
			switch (parse_integer(args[2])) {
				case 1: safe_str(unparse_integer(do_sensor_contacts(args[3], enactor)), buff, bp); break;
				case 2: safe_str(unparse_integer(do_eng_status(enactor)), buff, bp); break;
				case 3: safe_str(unparse_integer(do_helm_status(enactor)), buff, bp); break;
				case 4: safe_str(unparse_integer(do_tactical_status(enactor)), buff, bp); break;
				case 5: safe_str(unparse_integer(do_beam_status(enactor)), buff, bp); break;
				case 6: safe_str(unparse_integer(do_missile_status(enactor)), buff, bp); break;
				case 7: safe_str(unparse_integer(do_operations_status(enactor)), buff, bp); break;
				case 8: safe_str(unparse_integer(do_sensor_report(parse_integer(args[3]), enactor)), buff, bp); break;
				case 9: safe_str(unparse_integer(do_allocation_status(enactor)), buff, bp); break;
				case 10: safe_str(unparse_integer(do_damage_status(parse_integer(args[3]), args[4], enactor)), buff, bp); break;
				case 11: safe_str(unparse_integer(do_science_status(enactor)), buff, bp); break;
				case 12: safe_str(unparse_integer(do_fighter_status(enactor)), buff, bp); break;
				case 13: safe_str(unparse_integer(do_scanner_report(parse_integer(args[3]), args[4], enactor)), buff, bp); break;
				case 14: safe_str(unparse_integer(do_nebula_report(enactor)), buff, bp); break;
				case 15: safe_str(unparse_integer(do_border_report(enactor)), buff, bp); break;
				case 16: safe_str(unparse_integer(do_planet_report(args[3], 1, enactor)), buff, bp); break;
				case 17: safe_str(unparse_integer(do_planet_report(args[3], 0, enactor)), buff, bp); break;
				case 18: safe_str(do_sensor_contacts_bot(args[3], enactor), buff, bp); break;
				default: safe_str("#-1 NO SUCH FIELD SELECTION", buff, bp); break;
			} break;
		case 'v': /* variable */
			switch (args[1][2]) {
				case 'c': /* cochrane */
					a = parse_number(args[2]);
					if (a < 1.0) {
						cochrane = COCHRANE;
					} else if (a > COCHRANE * 1000.0) {
						cochrane = COCHRANE * 1000.0;
					} else
						cochrane = a;
					notify(executor, "Spacetime Cochrane constant set.");
					safe_str(unparse_number(cochrane), buff, bp); break; 
				case 'n': /* console */
					console = parse_dbref(args[3]);
					if (!GoodObject(console)) {
						safe_str("#-1 NOT A VALID DBREF", buff, bp); return; }
					switch (args[2][0]) {
						case 'c': /* communication */
							notify(executor, "Communications console parent dbref set.");
							console_communication = console; 
							addConsole("console_communication",console);
							break;
						case 'd': /* damage */
							notify(executor, "Damage Control console parent dbref set.");
							console_damage = console; 
							addConsole("console_damage",console);
							break;
						case 'e': /* engineering */
							notify(executor, "Engineering console parent dbref set.");
							console_engineering = console; 
							addConsole("console_engineering",console);
							break;
						case 'f': /* fighter */
							notify(executor, "Fighter/Shuttle console parent dbref set.");
							console_fighter = console; 
							addConsole("console_fighter",console);
							break;
						case 'h': /* helm */
							notify(executor, "Helm/Navigation console parent dbref set.");
							console_helm = console; 
							addConsole("console_helm",console);
							break;
						case 'm': /* monitor */
							notify(executor, "Monitor console parent dbref set.");
							console_monitor = console; 
							addConsole("console_monitor",console);
							break;
						case 'n': /* helm */
							notify(executor, "Helm/Navigation console parent dbref set.");
							console_helm = console; 
							addConsole("console_navigation",console);
							break;
						case 'o': /* operations */
							notify(executor, "Operations console parent dbref set.");
							console_operation = console; 
							addConsole("console_operation",console);
							break;
						case 's':
							switch (args[2][1]) {
								case 'c': /* science */
									notify(executor, "Science console parent dbref set.");
									console_science = console; 
									addConsole("console_science",console);
									break;
								case 'e': /* security */
									notify(executor, "Security console parent dbref set.");
									console_security = console; 
									addConsole("console_security",console);
									break;
								case 'h': /* fighter */
									notify(executor, "Fighter/Shuttle console parent dbref set.");
									console_fighter = console; 
									addConsole("console_shuttle",console);
									break;
								default: safe_str("#-1 NO SUCH FIELD SELECTION", buff, bp); break;
							} break;
						case 't':
							switch (args[2][1]) {
								case 'a': /* tactical */
									notify(executor, "Tactical/Weapons console parent dbref set.");
									console_tactical = console; 
									addConsole("console_tactical",console);
									break;
								case 'r': /* transporter */
									notify(executor, "Transporter console parent dbref set.");
									console_transporter = console;
									addConsole("console_transporter",console);									
									break;
								default : safe_str("#-1 NO SUCH FIELD SELECTION", buff, bp); break;
							} break;
						case 'w': /* tactical */
							notify(executor, "Tactical/Weapons console parent dbref set.");
							console_tactical = console; 
							addConsole("console_weapons",console);
							break;
						default: safe_str("#-1 NO SUCH FIELD SELECTION", buff, bp); break;
					}
					safe_str(unparse_dbref(console), buff, bp); break;
				default: safe_str("#-1 NO SUCH FIELD SELECTION", buff, bp); break;
			} break;
		case 'w': /* write */
			ship = parse_dbref(args[1]);
			if (!SpaceObj(ship) || !GoodObject(ship)) {
				safe_str("#-1 INVALID SPACE OBJECT", buff, bp); return; }
			safe_str(unparse_integer(do_space_db_write(ship, executor)), buff, bp); break;
		default: safe_str("#-1 NO SUCH FIELD SELECTION", buff, bp); break;
	}
	return;
}

/* ------------------------------------------------------------------------ */

FUNCTION(local_fun_cdb) /* cdb (<function>[,<field>[,<field>[,<field>[,<field>[,...]]]]]) */
{
	register int i, x, y, e;
	double range, max_range, freq;
	dbref zone;
	char *tptr[10];
	char const *tp, *tbuf;
	static char ibuf1[20], ibuf2[20];
	static char dbuf1[20], dbuf2[10], dbuf3[10];
	static char nbuf1[20], nbuf2[20], nbuf3[20];
	static char msg[BUFFER_LEN];
	char *mp = msg;
	ATTR *a;

	if (!Wizard(Owner(executor))) {
		safe_str("#-1 PERMISSION DENIED", buff, bp);
	} else {
		switch (args[0][0]) {
			case 'a': /* add to CDB <void> */
			 	if (Typeof(executor) != TYPE_THING) {
			 		safe_str("#-1 INVALID TYPE", buff, bp);
					return;
				}
				for (i = MIN_COMMS_OBJECTS; i <= MAX_COMMS_OBJECTS; ++i)
					if (cdb[i].object == executor) {
						safe_str("#-1 OBJECT ALREADY IN CDB", buff, bp);
						return;
					} else if (cdb[i].object == 0) {
						if (i > max_comms_objects)
							max_comms_objects = i;
						cdb[i].object = executor;
						cdb[i].lower = MIN_COMMS_FREQUENCY;
						cdb[i].upper = MAX_COMMS_FREQUENCY;
						safe_chr('1', buff, bp);
						notify(executor, tprintf("#%d/%d: object added.", cdb[i].object, i));
						return;
					}
				safe_str("#-1 OUT OF CDB SPACE", buff, bp);
				break;
			case 'b': /* bug fixing <void> */
				for (i = MIN_COMMS_OBJECTS; i <= MAX_COMMS_OBJECTS; ++i)
					if (cdb[i].object) {
						if (!GoodObject(cdb[i].object) || !Hasprivs(Owner(cdb[i].object))) {
							notify(executor, tprintf("#%d/%d: bad object deleted.", cdb[i].object, i));
							cdb[i].object = 0;
							cdb[i].lower = 0.0;
							cdb[i].upper = 0.0;
						} else {
							max_comms_objects = i;
							if (cdb[i].lower < MIN_COMMS_FREQUENCY) {
								notify(executor, tprintf("#%d/%d: bad lower bandpass fixed.", cdb[i].object, i));
								cdb[i].lower = MIN_COMMS_FREQUENCY;
							}
							if (cdb[i].upper > MAX_COMMS_FREQUENCY) {
								notify(executor, tprintf("#%d/%d: bad upper bandpass fixed.", cdb[i].object, i));
								cdb[i].upper = MAX_COMMS_FREQUENCY;
							}
							if (cdb[i].upper < cdb[i].lower) {
								notify(executor, tprintf("#%d/%d: bad bandpass fixed.", cdb[i].object, i));
								cdb[i].upper = cdb[i].lower;
							}
						}
					}
				safe_str(unparse_integer(max_comms_objects), buff, bp);
				break;
			case 'c': /* check the CDB <void> */
				for (i = MIN_COMMS_OBJECTS; i <= max_comms_objects; ++i)
					if (cdb[i].object == executor) {
						safe_str(unparse_integer(i), buff, bp);
						return;
					}
				safe_str("#-1 OBJECT NOT IN CDB", buff, bp);
				break;
			case 'd': /* delete from CDB <void> */
				for (i = MIN_COMMS_OBJECTS; i <= max_comms_objects; ++i)
					if (cdb[i].object == executor) {
						notify(executor, tprintf("#%d/%d: object deleted.", cdb[i].object, i));
						cdb[i].object = 0;
						cdb[i].lower = 0.0;
						cdb[i].upper = 0.0;
						safe_chr('1', buff, bp);
						return;
					}
				safe_str("#-1 OBJECT NOT IN CDB", buff, bp);
				break;
			case 'l': /* list <void> */
				for (i = MIN_COMMS_OBJECTS; i <= max_comms_objects; ++i)
					if (cdb[i].object == executor) {
						safe_str(tprintf("#%d %d %f %f", cdb[i].object, i, cdb[i].lower, cdb[i].upper), buff, bp);
						return;
					}
				safe_str("#-1 OBJECT NOT IN CDB", buff, bp);
				break;
			case 's': /* set bandpass <lower> <upper> */
				for (i = MIN_COMMS_OBJECTS; i <= max_comms_objects; ++i)
					if (cdb[i].object == executor) {
						cdb[i].lower = parse_number(args[1]);
						cdb[i].upper = parse_number(args[2]);
						if (cdb[i].lower < MIN_COMMS_FREQUENCY)
							cdb[i].lower = MIN_COMMS_FREQUENCY;
						if (cdb[i].upper > MAX_COMMS_FREQUENCY)
							cdb[i].upper = MAX_COMMS_FREQUENCY;
						if (cdb[i].upper < cdb[i].lower)
							cdb[i].upper = cdb[i].lower;
						safe_str(tprintf("%f %f", cdb[i].lower, cdb[i].upper), buff, bp);
						notify(executor, tprintf("#%d/%d: bandpass set to %f (lower) and %f (upper).", cdb[i].object, i, cdb[i].lower, cdb[i].upper));
						return;
					}
				safe_str("#-1 OBJECT NOT IN CDB", buff, bp);
				break;
			case 't': /* transmit <freq> <max range> <code> <message> */
				freq = parse_number(args[1]);
				if (freq < MIN_COMMS_FREQUENCY || freq > MAX_COMMS_FREQUENCY) {
					safe_str("#-1 BAD FREQUENCY VALUE", buff, bp);
					return;
				}
				max_range = parse_number(args[2]);
				if (max_range <= 0) {
					safe_str("#-1 BAD RANGE VALUE", buff, bp);
					return;
				}
				zone = Location(executor);
				if (Typeof(zone) != TYPE_ROOM) {
					zone = Location(zone);
					if (Typeof(zone) != TYPE_ROOM) {
						zone = Location(zone);
						if (Typeof(zone) != TYPE_ROOM) {
							safe_str("#-1 BAD LOCATION", buff, bp);
							return;
						} else
							zone = Zone(zone);
					} else
						zone = Zone(zone);
				} else
					zone = Zone(zone);
				if (!SpaceObj(zone)) {
					safe_str("#-1 BAD SPACE OBJECT", buff, bp);
					return;
				}
				a = atr_get(zone, SDB_ATTR_NAME);
				if (a == NULL) {
					safe_str("#-1 NO SDB NUMBER", buff, bp);
					return;
				}
				x = parse_integer(atr_value(a));
				if (!GoodSDB(x)) {
					safe_str("#-1 BAD SDB NUMBER", buff, bp);
					return;
				}
				/* save the stack */
				for (i = 0; i < 10; i++)
    				tptr[i] = global_eval_context.wenv[i];
				snprintf(ibuf1, sizeof(ibuf1), "%d", x);
				snprintf(dbuf1, sizeof(dbuf1), "#%d", enactor);
				snprintf(dbuf2, sizeof(dbuf2), "#%d", executor);
				snprintf(nbuf1, sizeof(nbuf1), "%g", freq);
				snprintf(nbuf2, sizeof(nbuf2), "%.0f", max_range);
    			global_eval_context.wenv[0] = nbuf1;
    			global_eval_context.wenv[1] = dbuf1;
				global_eval_context.wenv[2] = dbuf2;
				global_eval_context.wenv[4] = ibuf1;
				global_eval_context.wenv[6] = nbuf2;
				global_eval_context.wenv[8] = args[3];
				a = atr_get(executor, ENCRYPTION_ATTR_NAME);
				if (a == NULL) {
					safe_str(args[4], msg, &mp);
					e = 0;
				} else {
					safe_str(space_crypt_code(atr_value(a), args[4], 1), msg, &mp);
					e = 1;
				}
				*mp = '\0';
				for (i = MIN_COMMS_OBJECTS; i <= max_comms_objects; ++i)
					if (cdb[i].object) {
						if (freq < cdb[i].lower || freq > cdb[i].upper)
							continue;
						zone = Location(cdb[i].object);
						if (GoodObject(zone)) {
							if (Typeof(zone) != TYPE_ROOM) {
								zone = Location(zone);
								if (GoodObject(zone)) {
									if (Typeof(zone) != TYPE_ROOM) {
										zone = Location(zone);
										if (GoodObject(zone)) {
											if (Typeof(zone) != TYPE_ROOM) {
												continue;
											} else
												zone = Zone(zone);
										} else
											continue;
									} else
										zone = Zone(zone);
								} else
									continue;
							} else
								zone = Zone(zone);
						} else
							continue;
						if (!SpaceObj(zone))
							continue;
						a = atr_get(zone, SDB_ATTR_NAME);
						if (a == NULL)
							continue;
						y = parse_integer(atr_value(a));
						if (!GoodSDB(y))
							continue;
						range = sdb2range(x, y);
						if (range > max_range)
							continue;
						a = atr_get(cdb[i].object, EXECUTE_ATTR_NAME);
						if (a == NULL)
							continue;
						tp = tbuf = safe_atr_value(a);
						snprintf(dbuf3, sizeof(dbuf3), "#%d", cdb[i].object);
						snprintf(ibuf2, sizeof(ibuf2), "%d", y);
						snprintf(nbuf3, sizeof(nbuf3), "%.0f", range);
						global_eval_context.wenv[3] = dbuf3;
						global_eval_context.wenv[5] = ibuf2;
						global_eval_context.wenv[7] = nbuf3;
						if (e) {
							a = atr_get(cdb[i].object, ENCRYPTION_ATTR_NAME);
							if (a == NULL) {
								global_eval_context.wenv[9] = msg;
							} else
								global_eval_context.wenv[9] = space_crypt_code(atr_value(a), msg, 0);
						} else
							global_eval_context.wenv[9] = msg;
						process_expression(buff, bp, &tp, cdb[i].object, executor, enactor,
						  PE_DEFAULT, PT_DEFAULT, pe_info);
						free(tbuf);
					}
				/* restore the stack */
				for (i = 0; i < 10; i++)
    				global_eval_context.wenv[i] = tptr[i];
				safe_chr('1', buff, bp);
				notify(executor, "transmission sent.");
				break;
			default:
				safe_str("#-1 NO SUCH FIELD SELECTION", buff, bp);
				break;
		}
	}

	return;
}

/* ------------------------------------------------------------------------ */


/* ARGSUSED */
FUNCTION(local_fun_deg2rad)
{
	if (!is_number(args[0])) {
		safe_str(e_num, buff, bp);
	} else
		safe_str(unparse_number(parse_number(args[0]) / 57.29577951), buff, bp);
	return;
}

/* ------------------------------------------------------------------------ */

FUNCTION(fun_destination)
{
	/** This is used to evaluate the u-function DESTINATION on an exit with
	 * a VARIABLE (ambiguous) link.
	 */

  dbref thing;
  ATTR *a;
  char *s;

  thing = match_thing(executor, args[0]);
  if (!GoodObject(thing)) {
    safe_str(T("#-1 INVALID OBJECT"), buff, bp);
    return;
  }
  a = atr_get_noparent(thing, "FOLLOWERS");
  a = atr_get(thing, "DESTINATION");
  if (!a)
    a = atr_get(thing, "EXITTO");
  if (!a) {
    safe_dbref(Location(thing), buff, bp);
    return;	
  }
  s = atr_value(a);
  if (!s) {
    safe_dbref(Location(thing), buff, bp);
    return;	
  }
  process_expression(buff, bp, &s, thing, executor, executor, PE_DEFAULT, PT_DEFAULT, NULL);
  return;;
}

/* ------------------------------------------------------------------------ */

FUNCTION(fun_chown)
{
	if (!command_check_byname(executor, "@chown") || fun->flags & FN_NOSIDEFX) {
		safe_str(T(e_perm), buff, bp);
		return;
	}
	do_chown(executor, args[0], args[1], args[2]);
}

/* ------------------------------------------------------------------------ */

/* ARGSUSED */
FUNCTION(local_fun_ly2pc)
{
	if (!is_number(args[0])) {
		safe_str(e_num, buff, bp);
	} else
		safe_str(unparse_number(parse_number(args[0]) * LIGHTYEAR / PARSEC), buff, bp);
	return;
}

/* ------------------------------------------------------------------------ */

/* ARGSUSED */
FUNCTION(local_fun_ly2su)
{
	if (!is_number(args[0])) {
		safe_str(e_num, buff, bp);
	} else
		safe_str(unparse_number(parse_number(args[0]) * LIGHTYEAR), buff, bp);
	return;
}

/* ------------------------------------------------------------------------ */

/* ARGSUSED */
FUNCTION(local_fun_pc2ly)
{
	if (!is_number(args[0])) {
		safe_str(e_num, buff, bp);
	} else
		safe_str(unparse_number(parse_number(args[0]) * PARSEC / LIGHTYEAR), buff, bp);
	return;
}

/* ------------------------------------------------------------------------ */

/* ARGSUSED */
FUNCTION(local_fun_pc2su)
{
	if (!is_number(args[0])) {
		safe_str(e_num, buff, bp);
	} else
		safe_str(unparse_number(parse_number(args[0]) * PARSEC), buff, bp);
	return;
}

/* ------------------------------------------------------------------------ */

/* ARGSUSED */
FUNCTION(local_fun_rad2deg)
{
	if (!is_number(args[0])) {
		safe_str(e_num, buff, bp);
	} else
		safe_str(unparse_number(parse_number(args[0]) * 57.29577951), buff, bp);
	return;
}

/* ------------------------------------------------------------------------ */

/* ARGSUSED */
FUNCTION(local_fun_secs2sd)
{
	if (!is_integer(args[0])) {
		safe_str(e_ints, buff, bp);
	} else
		safe_str(unparse_number(((parse_integer(args[0]) - 558964800) / 31557.6) + 61153.7), buff, bp);
	return;
}

/* ------------------------------------------------------------------------ */

/* ARGSUSED */
FUNCTION(local_fun_sd2secs)
{
	double sd;
	
	if (!is_number(args[0])) {
		safe_str(e_num, buff, bp);
	} else {
		sd = parse_number(args[0]);
		if (sd > 80000.0) {
			safe_str("#-1 STARDATE TOO LARGE", buff, bp);
		} else if (sd < 0.0) {
			safe_str("#-1 STARDATE TOO SMALL", buff, bp);
		} else
			safe_str(unparse_integer((int) ((sd - 61153.7) * 31557.6) + 558964800), buff, bp);
	}
	return;
}

/* ------------------------------------------------------------------------ */
// Jarek recoded along with lookup_space to return nice lists of possible matches
/* ARGSUSED */
FUNCTION(local_fun_spacenum)
{
	dbref space[MAX_SPACE_OBJECTS];

	if (Hasprivs(executor))
	{
		lookup_space(args[0], space);
		if (space[0] == NOTHING)
			safe_str("#-1 SPACE OBJECT NOT FOUND", buff, bp);
		else if (space[1] != NOTHING)
			safe_str("#-2 AMBIGUOUS MATCH", buff, bp);
		else
			safe_dbref(space[0], buff, bp);
	}
	else
	    safe_str("#-1 PERMISSION DENIED", buff, bp);

	return;
}

/* ------------------------------------------------------------------------ */
// Jarek recoded along with lookup_space to return nice lists of possible matches
/* ARGSUSED */
FUNCTION(local_fun_spacenumlist)
{
	dbref space[MAX_SPACE_OBJECTS];
	int first = 1, i = 0;

	if (Hasprivs(executor))
	{
		lookup_space(args[0], space);
		if (space[0] == NOTHING)
			safe_str("#-1 SPACE OBJECT NOT FOUND", buff, bp);
		else
		{
			for (i = 0; space[i] != NOTHING; i++)
			{
				if (first) {
					first = 0;
					safe_dbref(space[i], buff, bp);
				} else {
					safe_chr(' ', buff, bp);
					safe_dbref(space[i], buff, bp);
				}
			}
			/*do
			{
				if (first)
					first = 0;
				else
					safe_chr(' ', buff, bp);
					safe_dbref(space[i], buff, bp);
				i++;
			}
			while(space[i] != NOTHING);*/
		}
	}
	else
	    safe_str("#-1 PERMISSION DENIED", buff, bp);

	return;
}

/* ------------------------------------------------------------------------ */

/* ARGSUSED */
FUNCTION(local_fun_sph2xyz)
{
	NVAL b, e, r;
	
	if (!is_number(args[0]) || !is_number(args[1]) || !is_number(args[2])) {
		safe_str(e_num, buff, bp);
	} else {
		b = parse_number(args[0]) * PI / 180;
		e = parse_number(args[1]) * PI / 180;
		r = parse_number(args[2]) ;
		safe_str(unparse_number(cos(b) * cos(e) * r), buff, bp);
		safe_chr(' ', buff, bp);
		safe_str(unparse_number(sin(b) * cos(e) * r), buff, bp);
		safe_chr(' ', buff, bp);
		safe_str(unparse_number(sin(e) * r), buff, bp);
	}
	return;
}

/* ------------------------------------------------------------------------ */

/* ARGSUSED */
FUNCTION(local_fun_su2ly)
{
	if (!is_number(args[0])) {
		safe_str(e_num, buff, bp);
	} else
		safe_str(unparse_number(parse_number(args[0]) / LIGHTYEAR), buff, bp);
	return;
}

/* ------------------------------------------------------------------------ */

/* ARGSUSED */
FUNCTION(local_fun_su2pc)
{
	if (!is_number(args[0])) {
		safe_str(e_num, buff, bp);
	} else
		safe_str(unparse_number(parse_number(args[0]) / PARSEC), buff, bp);
	return;
}

/* ------------------------------------------------------------------------ */

/* ARGSUSED */
FUNCTION(local_fun_xyz2sph)
{
	NVAL b, e, r, x, y, z, xy;
	
	if (!is_number(args[0]) || !is_number(args[1]) || !is_number(args[2])) {
		safe_str(e_num, buff, bp);
	} else {
		x = parse_number(args[0]);
		y = parse_number(args[1]);
		z = parse_number(args[2]);
		xy = sqrt(x * x + y * y);
		r = sqrt(x * x + y * y + z * z);
#ifndef HAS_IEEE_MATH
  		/* You can overflow, which is bad. */
		if ((r < 0.0) || (xy < 0.0)) {
			safe_str("#-1 OVERFLOW ERROR", buff, bp);
			return;
  		}
#endif /* HAS_IEEE_MATH */
		if (y == 0.0) {
			if (x == 0.0) {
				b = 0.0;
			} else if (x > 0.0) {
				b = 0.0;
			} else
				b = 180.0;
		} else if (x == 0.0) {
			if (y > 0.0) {
				b = 90.0;
			} else
				b = 270.0;
		} else if (x > 0.0) {
			if (y > 0.0) {
				b = atan(y / x) * 180.0 / PI;
			} else
				b = atan(y / x) * 180.0 / PI + 360.0;
		} else if (x < 0.0) {
			b = atan(y / x) * 180.0 / PI + 180.0;
		} else 
			b = 0.0;
		
		if (xy == 0.0) {
			if (z == 0.0) {
				e = 0.0;
			} else if (z > 0.0) {
				e = 90.0;
			} else
				e = 270.0;
		} else if (z > 0.0) {
			e = atan(z / xy) * 180.0 / PI;
		} else if (z < 0.0) {
			e = atan(z / xy) * 180.0 / PI + 360;
		} else
			e = 0.0;
		
		safe_str(unparse_number(b), buff, bp);
		safe_chr(' ', buff, bp);
		safe_str(unparse_number(e), buff, bp);
		safe_chr(' ', buff, bp);
		safe_str(unparse_number(r), buff, bp);
	}
	return;
}
/* ------------------------------------------------------------------------ */

FUNCTION(local_fun_coords)
{
   int sdb_num = parse_integer(args[0]);
   int c_type = parse_integer(args[1]);
   
   if (!GoodSDB(sdb_num))
	safe_str("#-1 SDB OUT OF RANGE", buff, bp);
   else {
	if (c_type) 
	    safe_str(tprintf("%f %f %f", sdb[sdb_num].coords.x / PARSEC, sdb[sdb_num].coords.y / PARSEC, 
sdb[sdb_num].coords.z / PARSEC), buff, bp);
	else
	    safe_str(tprintf("%f %f %f", sdb[sdb_num].coords.x, sdb[sdb_num].coords.y, sdb[sdb_num].coords.z), buff, 
bp);
   }
}

/* ------------------------------------------------------------------------ */

FUNCTION(local_fun_inrange)
{
	register int i;
	int r;
	double q, the_range;
	static char buffer[1000];
	char *relay, *s;
	ATTR *a;

	if (args[2] == NULL)
		args[2] = unparse_integer(0);
	if (args[3] == NULL)
		args[3] = NULL;
	if (args[4] == NULL)
		args[4] = NULL;

	n = parse_integer(args[0]);
	q = parse_number(args[1]);
	r = parse_integer(args[2]);
	s = args[3];

	if (Hasprivs(executor) || has_power_by_name(executor, "SDB-OK", NOTYPE) || has_power_by_name(executor, "SDB-READ", NOTYPE))
	{
		if (GoodSDB(n))
		{
			buffer[0] = '\0';

			switch (r)
			{
				case 0: q = pc2su(q); break;
				case 1: q = q; break;
				default: q = pc2su(q); break;
			}

			for	(i = MIN_SPACE_OBJECTS; i <= MAX_SPACE_OBJECTS; ++i)
			{
				if (n == i)
				{
					continue;
				}
				else if (GoodSDB(i)) /* Additional if added by Anyasha - STDW */
				{
					the_range =sdb2range(n, i);

					if (args[3] != NULL)
					{
						a = atr_get(sdb[i].object, upcasestr(args[3]));

						if (a != NULL)
						{
							relay = safe_atr_value(a);

							if(the_range <= q && relay != NULL)
							{
								if (args[4] != NULL)
								{
									if (parse_number(relay) == parse_number(args[4]))
									{
										strncat(buffer, tprintf("#%d|%d|%f", sdb[i].object, i, sdb2range(n, i)), sizeof(buffer) - 1);
										strncat(buffer, " ", sizeof(buffer));
									}
								}
								else
								{
									strncat(buffer, tprintf("#%d|%d|%f", sdb[i].object, i, sdb2range(n, i)), sizeof(buffer) - 1);
									strncat(buffer, " ", sizeof(buffer));
								}
							}
						}
						a = NULL;
					}
					else
					{
						if(the_range <= q)
						{
							strncat(buffer, tprintf("#%d|%d|%f", sdb[i].object, i, sdb2range(n, i)), sizeof(buffer) - 1);
							strncat(buffer, " ", sizeof(buffer));
						}
					}
				}
				else continue; /* Statement added by Anyasha - STDW */
			}
			safe_str(buffer, buff, bp);
		}
		else
		{
			safe_str("#-1 SDB OUT OF RANGE", buff, bp);
		}
	}
	else
		safe_str("#-1 PERMISSION DENIED", buff, bp);

	return;
}

/* ------------------------------------------------------------------------ */

FUNCTION(local_fun_border)
{
	if (!args[0] || !*args[0]) {
		safe_str("#-1 NO FUNCTION NAME GIVEN", buff, bp);
		return;
	}
	
	nargs -= 1;
	
	switch (args[0][0]) {
		case 'a': 
				if (nargs != 6) {
					safe_str(tprintf("#-1 INVALID NUMBER OF ARGUMENTS %d REQUIRES 5", nargs), buff, bp);
					return;
				}
				addNewBorder(parse_integer(args[1]), args[2], parse_number(args[3]), parse_number(args[4]), parse_number(args[5]), parse_number(args[6])); 
			break;
		case 'd': 
				if (nargs != 1) {
					safe_str(tprintf("#-1 INVALID NUMBER OF ARGUMENTS %d REQUIRES 1", nargs), buff, bp);
					return;
				}
				safe_str(deleteBorder(parse_integer(args[1])), buff, bp); 
			break;
		case 'e': 
				if (nargs != 3) {
					safe_str(tprintf("#-1 INVALID NUMBER OF ARGUMENTS %d REQUIRES 3", nargs), buff, bp);
					return;
				}
				safe_str(edit_border(parse_integer(args[1]), args[2], args[3]), buff, bp); 
			break;
		case 'l': safe_str(list_borders(), buff, bp); break;
	}
	
	return;
}

/* ------------------------------------------------------------------------ */

void free_spaceconsole(void *ptr) {
	space_consoles *sc = (space_consoles *) ptr;
	mush_free(sc->console_dbref, "console_dbref");
	mush_free(sc->console_name, "console_name");
	mush_free(sc, "space_consoles");
}

FUNCTION(local_fun_consolemsg)
{
	ATTR *a, *b;
	dbref console, user, parent;
	char *q, *pq, *msg, *the_console, *c_pq, *console_list;
	int x, index, result;
	char *consoles[BUFFER_LEN / 2];
	space_consoles *sc;
	
	char *buffer;
	
	x = parse_integer(args[0]);
	
	result = list2arr(consoles, BUFFER_LEN / 2, args[1], ' ');
	
	msg = (char *) mush_strdup(args[2], "console_message");
	
	if (!GoodObject(sdb[x].object) || !SpaceObj(sdb[x].object)) {
		write_spacelog(sdb[x].object, sdb[x].object, "BUG: invalid zone to zemit to.");
		return;
	}

	a = atr_get(sdb[x].object, CONSOLE_ATTR_NAME);
	if (a != NULL) {
		q = safe_atr_value(a);
		pq = trim_space_sep(q, ' ');
		while (pq) {
			console = parse_dbref(split_token(&pq, ' '));
					
			if ( console != NULL )
			{			
				for ( index = 0; index < result; index++) {
					c_pq = tprintf("console_%s", consoles[index]);
				
					if ( c_pq != NULL )
					{
						sc = hashfind(c_pq, &aspace_consoles);
				
						if (GoodObject(console) && sc != NULL) {
							parent = Parent(console);
							if ( parent == sc->console_dbref ) {
								b = atr_get(console, CONSOLE_USER_ATTR_NAME);
								if (b != NULL) {
									user = parse_dbref(atr_value(b));
									if (GoodObject(user)) {
										notify(user, msg);
									}
								}
							}
						}
					}
				}
			}
		}
		free(q);
		mush_free(msg, "console_message");
	}
	
	return;
}

/* ------------------------------------------------------------------------ */

void addConsole(char *console_name, dbref console_dbref)
{
	space_consoles *sc = NULL;
	
	sc = mush_malloc(sizeof(space_consoles), "space_consoles");
	sc->console_name = mush_strdup(console_name, "console_name");
	sc->console_dbref = console_dbref;
	
	hash_add(&aspace_consoles, sc->console_name, sc);
}

/* ------------------------------------------------------------------------ */

void setupAspaceFunctions()
{
	function_add("CONSOLEMSG", local_fun_consolemsg, 3, 3, FN_ADMIN);
	function_add("BORDER", local_fun_border, 1, 7, FN_ADMIN);
	function_add((char *) "CDB", local_fun_cdb, 1, 5, FN_REG);
	function_add("INRANGE", local_fun_inrange, 2, 5, FN_REG);
	function_add("COORDS", local_fun_coords, 2, 2, FN_REG);
	function_add("DEG2RAD", local_fun_deg2rad, 1, 1, FN_REG);
	function_add((char *) "DESTINATION", fun_destination, 1, 1, FN_REG);
	function_add((char *) "CHOWN", fun_chown, 2, 3, FN_WIZARD);
	function_add("LY2PC", local_fun_ly2pc, 1, 1, FN_REG);
	function_add("LY2SU", local_fun_ly2su, 1, 1, FN_REG);
	function_add("PC2LY", local_fun_pc2ly, 1, 1, FN_REG);
	function_add("PC2SU", local_fun_pc2su, 1, 1, FN_REG);
	function_add("RAD2DEG", local_fun_rad2deg, 1, 1, FN_REG);
	function_add("SD2SECS", local_fun_sd2secs, 1, 1, FN_REG);
	function_add("SDB", local_fun_sdb, 1, 9, FN_REG);
	function_add("SECS2SD", local_fun_secs2sd, 1, 1, FN_REG);
	function_add("SPACENUM", local_fun_spacenum, 1, 1, FN_REG);
	function_add("SPACENUMLIST", local_fun_spacenumlist, 1, 1, FN_REG);
	function_add("SPH2XYZ", local_fun_sph2xyz, 3, 3, FN_REG);
	function_add("SU2LY", local_fun_su2ly, 1, 1, FN_REG);
	function_add("SU2PC", local_fun_su2pc, 1, 1, FN_REG);
	function_add("XYZ2SPH", local_fun_xyz2sph, 3, 3, FN_REG);
}

void setupAspaceFlags()
{
	struct pennmush_flag_info* pFlagInfo;

	for (pFlagInfo = aspace_flag_table; pFlagInfo->name; pFlagInfo++)
	{
		add_flag(pFlagInfo->name, pFlagInfo->letter, pFlagInfo->type, 
			pFlagInfo->perms, pFlagInfo->negate_perms);
	}
}

void setupAspacePowers()
{
   struct pennmush_flag_info* pFlagInfo;
   
   for (pFlagInfo = aspace_power_table; pFlagInfo->name; pFlagInfo++)
     {
	add_power(pFlagInfo->name, pFlagInfo->letter, pFlagInfo->type,
		  pFlagInfo->perms, pFlagInfo->negate_perms);
     }
}

void initSpace()
{	
	border_map = NULL;
	border_map = im_new();

	hash_init(&aspace_consoles, 256, free_borderinfo);
	
	(void) setupAspaceFunctions();
	(void) setupAspaceFlags();
	(void) dump_space(GOD);
	(void) open_spacelog();
}

