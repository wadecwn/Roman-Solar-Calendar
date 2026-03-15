#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define NUM_MONTHS 13
#define DAYS_PER_MONTH 28
#define DAYS_PER_YEAR 364 // 13 * 28, excludes Day Out of Time

// Gregorian leap year rules (full Proleptic Gregorian / same as your calendar)
#define IS_LEAP_YEAR(y) (((y) % 4 == 0 && (y) % 100 != 0) || ((y) % 400 == 0))

// Roman Solar calendar's leap day falls between Sextilis 14 and 15 (month index
// 5, 0-based) That is day offset 5*28 + 14 = 154 from Aprilis 1 (0-based: after
// day index 153)
#define LEAP_DAY_OFFSET 154 // inserted AFTER the 154th day (Sextilis 14)

const char *MONTH_NAMES[NUM_MONTHS] = {
    "Aprilis",    // I
    "Maius",      // II
    "Iunius",     // III
    "Quadtilis",  // IV
    "Quintilis",  // V
    "Sextilis",   // VI   <- leap day between day 14 & 15
    "September",  // VII
    "October",    // VIII
    "November",   // IX
    "December",   // X
    "Undecember", // XI
    "Ianuarius",  // XII
    "Februarius"  // XIII
};

const char *MONTH_NUMERALS[NUM_MONTHS] = {"I",  "II",  "III",  "IV", "V",
                                          "VI", "VII", "VIII", "IX", "X",
                                          "XI", "XII", "XIII"};

// ── Struct to hold Roman Solar calendar date ──────────────────────────
typedef struct {
  int year;    // Solar calendar year (same as Gregorian year of spring equinox)
  int month;   // 0-based month index (0 = Aprilis)
  int day;     // 1-based day of month (1–28)
  int is_doot; // 1 if this is the Day Out of Time
  int is_leap_day; // 1 if this is the intercalary leap day
} SolarDate;

// ── Compute the Julian Day Number for a Gregorian date ───────────────────────
// This lets us do simple integer arithmetic on calendar dates.
static long gregorian_to_jdn(int y, int m, int d) {
  // Algorithm from https://en.wikipedia.org/wiki/Julian_day
  long a = (14 - m) / 12;
  long Y = y + 4800 - a;
  long M = m + 12 * a - 3;
  return d + (153 * M + 2) / 5 + 365 * Y + Y / 4 - Y / 100 + Y / 400 - 32045;
}

// ── Convert a Gregorian date to solar calendar date ────────────────────
SolarDate gregorian_to_solar(int g_year, int g_month, int g_day) {
  SolarDate result = {0};

  // The Solar year starts on March 20 of the Gregorian year.
  // If the Gregorian date is before March 20, it belongs to the PREVIOUS Solar
  // year.
  int solar_year = g_year;
  if (g_month < 3 || (g_month == 3 && g_day < 20)) {
    solar_year = g_year - 1;
  }

  // Anchor: Aprilis 1 = March 20 of solar_year
  long jdn_aprilis1 = gregorian_to_jdn(solar_year, 3, 20);
  long jdn_today = gregorian_to_jdn(g_year, g_month, g_day);

  long day_offset = jdn_today - jdn_aprilis1; // 0-based days since Aprilis 1

  int leap = IS_LEAP_YEAR(solar_year);

  // Day Out of Time = day 364 (0-based) in normal years, day 365 in leap years
  long doot_offset = leap ? 365 : 364;

  if (day_offset == doot_offset) {
    // Day Out of Time
    result.year = solar_year;
    result.is_doot = 1;
    return result;
  }

  if (leap && day_offset == LEAP_DAY_OFFSET) {
    // The intercalary leap day (between Sextilis 14 and 15)
    result.year = solar_year;
    result.is_leap_day = 1;
    result.month = 5; // Sextilis (0-based)
    result.day = 14;  // It falls after day 14
    return result;
  }

  // In a leap year, days after the leap day insertion need to be shifted back
  // by 1
  long adjusted_offset = day_offset;
  if (leap && day_offset > LEAP_DAY_OFFSET) {
    adjusted_offset = day_offset - 1;
  }

  result.year = solar_year;
  result.month = (int)(adjusted_offset / DAYS_PER_MONTH);
  result.day = (int)(adjusted_offset % DAYS_PER_MONTH) + 1; // 1-based

  return result;
}

// ── Pretty-print a custom calendar date ──────────────────────────────────────
void print_solar_date(SolarDate cd) {
  if (cd.is_doot) {
    printf("*** The Day Out of Time *** (Solar Year %d)\n", cd.year);
    return;
  }
  if (cd.is_leap_day) {
    printf("*** Leap Day *** (between %s 14 & 15, Solar Year %d)\n",
           MONTH_NAMES[cd.month], cd.year);
    return;
  }
  printf("%s %d, Year %d  [Month %s]\n", MONTH_NAMES[cd.month], cd.day, cd.year,
         MONTH_NUMERALS[cd.month]);
}

// ── Main ─────────────────────────────────────────────────────────────────────
int main(void) {

  /* ── Print calendar overview ──
  printf("=== The 13-Month Roman Solar Calendar ===\n");
  printf("%-6s %-15s %s\n", "No.", "Month", "Days");
  printf("------------------------------------\n");
  for (int i = 0; i < NUM_MONTHS; i++) {
      printf("%-6s %-15s %d\n", MONTH_NUMERALS[i], MONTH_NAMES[i],
  DAYS_PER_MONTH);
  }
  printf("%-6s %-15s %d\n", "--", "Day Out of Time", 1);
  printf("------------------------------------\n");
  printf("%-6s %-15s %d / %d (leap)\n", "Total", "", 365, 366);
  printf("\n"); */

  // ── Get and display current Gregorian date ──
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  int g_year = t->tm_year + 1900; // fix: was + 2000
  int g_month = t->tm_mon + 1;    // 1-based
  int g_day = t->tm_mday;

  // printf("Current Gregorian Date: %02d-%02d-%04d\n", g_month, g_day, g_year);

  // ── Convert to Solar calendar ──
  SolarDate today = gregorian_to_solar(g_year, g_month, g_day);

  // printf("Current Roman Solar Calendar Date:   ");
  print_solar_date(today);
  // printf("\n");

  /* ── Demo: walk through a full Solar year to verify conversion ──
  printf("=== Full Year Walk: Roman Solar Year 2025 (Mar 20 2025 – Mar 19 2026)
  ===\n"); printf("%-20s %-6s | %-12s\n", "Solar Date", "Day", "Gregorian");
  printf("----------------------------------------------\n");

  // Use time_t arithmetic to walk the year — avoids JDN reverse conversion
  bugs.
  // Anchor: Aprilis 1 = March 20, 2025 at midnight UTC
  int walk_year = 2025;
  int leap = IS_LEAP_YEAR(walk_year);
  int total_days = leap ? 366 : 365;

  struct tm anchor = {0};
  anchor.tm_year = walk_year - 1900;
  anchor.tm_mon  = 2;   // March (0-based)
  anchor.tm_mday = 20;
  anchor.tm_hour = 12;  // noon to avoid DST edge cases
  time_t anchor_t = mktime(&anchor);

  for (int offset = 0; offset < total_days; offset++) {
      // Add offset days in seconds
      time_t cur_t = anchor_t + (time_t)offset * 86400;
      struct tm *cur = localtime(&cur_t);

      int yy = cur->tm_year + 1900;
      int mm = cur->tm_mon + 1;
      int dd = cur->tm_mday;

      SolarDate cd = gregorian_to_solar(yy, mm, dd);

      // Print first day, last day, and special days only
      if (cd.is_doot || cd.is_leap_day || cd.day == 1 || cd.day == 28) {
          char greg_buf[32];
          snprintf(greg_buf, sizeof(greg_buf), "%04d-%02d-%02d", yy, mm, dd);

          if (cd.is_doot) {
              printf("%-20s %-6s | %s\n", "Day Out of Time", "---", greg_buf);
          } else if (cd.is_leap_day) {
              printf("%-20s %-6s | %s\n", "** Leap Day **", "---", greg_buf);
          } else {
              char custom_buf[32];
              snprintf(custom_buf, sizeof(custom_buf), "%s %d",
                       MONTH_NAMES[cd.month], cd.day);
              printf("%-20s %-6d | %s\n", custom_buf, cd.day, greg_buf);
          }
      }
  }*/

  return 0;
}