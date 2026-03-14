#!/bin/bash
# /home/13monthcalendar/update_date.sh

DATE_OUTPUT=$(/home/13monthcalendar/calendar)

cat > /var/www/calendar/index.html <<EOF
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Roman Solar Calendar</title>
    <style>
        .Title {
            font-weight: bold;
        }

        .Tab {
            margin-left: 20px;
        }
    </style>
</head>

<body>
    <h1 class="Title">13 Month - 28 Day Calendar "Roman Solar Calendar"</h1>
    <h2>Todays Date is: $DATE_OUTPUT</h2>

    <table>
        <tr>
            <td>
                test1
            </td>
        </tr>
        <tr>
            <td>
                test 2
            </td>
        </tr>
    </table>

    <h3>Roman Solar Calendar Info:</h3>
    <p class="Title">
        There are 28 days for each month.
    </p>
    <p class="Tab">
        - Each month has 4 days (Sun-Mon-Tue-Wed-Thr-Fri-Sat) each. <br>
        - Months start on Sunday and end on the 4th Saturday.<br>
        - The First day of the Year (Aprilis 1st) is the Spring Equinox. <br>
        - The Last day of the Year (Day Out of Time/Day after Februarius 28th) falls out of the calendars range. <br>
        - Leap Year occurs, following the rule stated <a href="https://en.wikipedia.org/wiki/Leap_year"
            target="_blank">here.</a> The Leap Day falls in
        between
        Sextilis 14th & 15th, the middle of the year, and is not recorded on the calendar.
    </p>
    <p class="Title">
        Months:
    </p>
    <p class="Tab">
        I - Aprilis, 1st - Spring Equinox <br>
        II - Maius <br>
        III - Lunius <br>
        IV - Quadtilis, 7th - Summer Solstice <br>
        V - Quintilis <br>
        VI - Sextilis, **** Every 4 Years Between The 14th & 15th, Leap Day Occurs **** <br>
        VII - September, 14th - Fall/Autumn Equinox <br>
        VIII - October <br>
        IX - November <br>
        X - December, 21st - Winter Solstice <br>
        XI - Undecember <br>
        XII - Lanuarius <br>
        XIII - Februarius <br>
        - - The Day Out of Time/Renewal/"New Years Eve" - -
    </p>

    <a href="https://github.com/" target="_blank">Github</a>
</body>

</html>
EOF