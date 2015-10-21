/* space_variables.c */

#include "config.h"
#include "space.h"

/* ------------------------------------------------------------------------ */

struct space_database_t sdb[MAX_SPACE_OBJECTS + 1];
struct comms_database_t cdb[MAX_COMMS_OBJECTS + 1];
double cochrane = COCHRANE;
int n;
int max_space_objects = MIN_SPACE_OBJECTS;
int max_comms_objects = MIN_COMMS_OBJECTS;

dbref console_security = CONSOLE_SECURITY;
dbref console_helm = CONSOLE_HELM;
dbref console_engineering = CONSOLE_ENGINEERING;
dbref console_operation = CONSOLE_OPERATION;
dbref console_science = CONSOLE_SCIENCE;
dbref console_damage = CONSOLE_DAMAGE;
dbref console_communication = CONSOLE_COMMUNICATION;
dbref console_tactical = CONSOLE_TACTICAL;
dbref console_transporter = CONSOLE_TRANSPORTER;
dbref console_monitor = CONSOLE_MONITOR;
dbref console_fighter = CONSOLE_FIGHTER;

/* ------------------------------------------------------------------------ */

const char *shield_name[] = {
    "Forward shield",
    "Starboard shield",
    "Aft shield",
    "Port shield",
    "Dorsal shield",
    "Ventral shield"
};

const char *cloak_name[] = {
    "Other",
    "Cloak"
};

const char *type_name[] = {
    "None",
    "Ship",
    "Base",
    "Planet",
    "Anomaly",
    "Star",
	"Asteroid",
	"Debris",
	"Computer-Core",
	"Corpse",
	"Player",
	"Other",
};

const char *beam_name[] = {
    "Beam Weapon",
	"Phaser Emitter",
	"Phaser Bank",
	"Phaser Cannon",
	"Phaser Array",
	"Disruptor",
	"Disruptor Bank",
	"Disruptor Cannon",
	"Disruptor Array",
	"Laserbeam",
	"Fusion Laser",
	"Nadion Beam",
	"Plasma Beam",
	"Polaron Beam",
	"Meson Beam",
	"Antiproton Beam",
	"Ion Beam",
	"Fusion Cutter",
	"Fusion Beam",
	"Mass Driver",
	"Phase Disruptor",
	"Jolt Cannon",
	"Death Ray",
};

const char *missile_name[] = {
    "Missile Weapon",
	"Photon Torpedo",
	"Photon Cannon",
	"Particle Cannon",
	"Plasma Torpedo",
	"Fusion Missile",
	"Fission Missile",
	"Missile",
	"Torpedo",
	"Propelled Explosive",
	"Impact Cannon",
	"Reactory Array",
	"Neutron Torpedo",
	"Proton Torpedo",
	"FTL Wave Generator",
	"Antimatter Torpedo",
	"Ballistic Missile",
	"Railgun",
	"Destruct-o-matic",
};

const char *quadrant_name[] = { /* Changed order of quad names to match Star Charts book - Anya STDW */
    "Alpha",
    "Gamma",
    "Beta",
    "Delta" 
};

const char *system_name[] = {
	"Superstructure",
	"Fusion Reactor",
	"Batteries",
	"Beam Weapon",
	"Cloaking Device",
	"EW Systems",
	"Impulse Drive",
	"LR Sensors",
	"M/A Reactor",
	"Missile Weapon",
	"Shield",
	"SR Sensors",
	"Tractor Beams",
	"Transporters",
	"Warp Drive"
};

double repair_mult[] = {
    32.0, 4.0, 2.0, 1.0, 8.0, 8.0, 4.0, 8.0, 8.0, 2.0, 1.0, 8.0, 1.0, 8.0, 4.0
};

const char *damage_name[] = {
	"No Damage",
	"Patched Damage",
	"Minor Damage",
	"Light Damage",
	"Moderate Damage",
	"Heavy Damage",
	"Severe Damage",
	"Inoperative",
	"Destroyed"
};
/* ------------------------------------------------------------------------ */
