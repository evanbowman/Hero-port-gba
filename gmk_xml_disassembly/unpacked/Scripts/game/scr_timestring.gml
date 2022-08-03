//Translate global.seconds into local.timestring
timesecs=global.seconds;
timehours=floor((timesecs-(timesecs mod 3600))/3600);
timesecs-=timehours*3600;
timemins=floor((timesecs-(timesecs mod 60))/60);
timesecs=timesecs mod 60;
timestring="";
if (timehours<10)
    timestring+="0";
timestring+=string(timehours)+":";
if (timemins<10)
    timestring+="0";
timestring+=string(timemins)+".";
if (timesecs<10)
    timestring+="0";
timestring+=string(timesecs);