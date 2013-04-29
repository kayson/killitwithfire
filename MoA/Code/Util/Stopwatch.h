/*************************************************************************************************
 *
 * Modeling and animation (TNM079) 2007
 * Code base for lab assignments. Copyright:
 *   Gunnar Johansson (gunnar.johansson@itn.liu.se)
 *   Ken Museth (ken.museth@itn.liu.se)
 *   Michael Bang Nielsen (bang@daimi.au.dk)
 *   Ola Nilsson (ola.nilsson@itn.liu.se)
 *   Andreas Söderström (andreas.soderstrom@itn.liu.se)
 *
 *************************************************************************************************/
#ifndef _STOPWATCH_H
#define _STOPWATCH_H

#ifndef WIN32
  #include <sys/time.h>
#else
  #include <time.h>
#endif

//-----------------------------------------------------------------------------
inline static double seconds(bool highPrecision)
{
  // High precision routine (wraps around after ~72 hours)
  if (highPrecision){
    #ifdef __linux
      struct timeval  tv;
      struct timezone tz;
      gettimeofday( &tv, &tz);
      return (double)(tv.tv_usec)*1.0e-6 + (double)(tv.tv_sec);
    #else
      const double secs_per_tick = 1.0 / CLOCKS_PER_SEC;
      return ( (double) clock() ) * secs_per_tick;
    #endif
  }
  else{
    // Low precision routine (counts seconds)
    return (double)(time(NULL));
  }
}

//-----------------------------------------------------------------------------
class Stopwatch {
private:
  bool mHighPrecision;
  int running_;
  double start_time_;
  double total_;

public:
  inline Stopwatch();

  inline void highPrecision(bool p) {mHighPrecision = p;}
  inline void start();
  inline double stop();
  inline double read();
  inline void resume(bool setStartTime = true);
  inline int running();
};

//-----------------------------------------------------------------------------
inline Stopwatch::Stopwatch() : mHighPrecision(true), running_(0), start_time_(0), total_(0) {}

//-----------------------------------------------------------------------------
inline void Stopwatch::start() {
  running_ = 1;
  total_ = 0;
  start_time_ = seconds(mHighPrecision);
}

//-----------------------------------------------------------------------------
inline double Stopwatch::stop() {
  if (running_) {
    total_ += (seconds(mHighPrecision) - start_time_);
    running_ = 0;
  }
  return total_;
}

//-----------------------------------------------------------------------------
inline void Stopwatch::resume(bool setStartTime) {
  if (!running_) {
      {
        start_time_ = seconds(mHighPrecision);
      }
      running_ = 1;
  }
}

//-----------------------------------------------------------------------------
inline double Stopwatch::read() {
  if (running_) {
    stop();
    resume(false);
  }
  return total_;
}


#endif
