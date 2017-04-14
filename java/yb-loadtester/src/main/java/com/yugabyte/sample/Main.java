// Copyright (c) YugaByte, Inc.

package com.yugabyte.sample;

import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;

import com.yugabyte.sample.apps.AppBase;
import com.yugabyte.sample.apps.AppConfig;
import com.yugabyte.sample.common.CmdLineOpts;
import com.yugabyte.sample.common.IOPSThread;
import com.yugabyte.sample.common.IOPSThread.IOType;

/**
 * Main entry point for the sample applications. This class spawns a bunch of IO threads which run
 * the various apps. The number of threads depend on the app defaults and the command line overrides
 * if any.
 *
 *
 * Layout of the source code
 * =========================
 * <this directory>
 * |__ Main.java
 * |__ apps/
 *     |__ AppBase.java              : Base class for all the apps. Has helper methods for creating
 *                                     Cassandra and Redis clients.
 *     |__ AppConfig.java            : Configuration for all the apps.
 *     |__ CassandraHelloWorld.java  : The simplest app that writes one employee record. Good
 *                                     starting point to understand how to write a Cassandra app.
 *     |__ CassandraKeyValue.java    : Simple key-value Cassandra app.
 *     |__ CassandraStockTicker.java : Sample stock-ticker app.
 *     |__ CassandraTimeseries.java  : Sample timeseries workload app/
 *     |__ RedisKeyValue.java     : Simple Redis app, good starting point for writing Redis apps.
 *
 *
 * Usage
 * ======
 * Just run the executable jar with no options (or a --help option) and it will print the usage
 * along with a short description about the apps.
 */
public class Main {
  private static final Logger LOG = Logger.getLogger(Main.class);
  // Helper class to parse command line options specified by the user if any, and return the
  // appropriate values for various options.
  CmdLineOpts cmdLineOpts;
  // The app to run.
  AppBase app;
  // The list of iops threads that will be spawned.
  List<IOPSThread> iopsThreads = new ArrayList<IOPSThread>();

  static {
    Logger.getRootLogger().setLevel(Level.ERROR);
    Logger.getLogger("com.yugabyte.sample").setLevel(Level.INFO);
  }

  public Main(CmdLineOpts cmdLineOpts) {
    this.cmdLineOpts = cmdLineOpts;
    this.app = cmdLineOpts.getAppInstance();
  }

  public void run() {
    try {
      // If this is a simple app, run it and return.
      if (app.appConfig.appType == AppConfig.Type.Simple) {
        app.run();
        return;
      }

      // Create the table if needed.
      if (!cmdLineOpts.getReuseExistingTable()) {
        app.dropTable();
      }
      app.createTableIfNeeded();

      // Create the reader and writer threads.
      int idx = 0;
      for (; idx < cmdLineOpts.getNumWriterThreads(); idx++) {
        iopsThreads.add(new IOPSThread(idx, cmdLineOpts.getAppInstance(), IOType.Write));
      }
      for (; idx < cmdLineOpts.getNumWriterThreads() + cmdLineOpts.getNumReaderThreads();
           idx++) {
        iopsThreads.add(new IOPSThread(idx, cmdLineOpts.getAppInstance(), IOType.Read));
      }

      // Start the reader and writer threads.
      for (IOPSThread iopsThread : iopsThreads) {
        iopsThread.start();
      }

      // Wait for the various threads to exit.
      while (!iopsThreads.isEmpty()) {
        try {
          iopsThreads.get(0).join();
          iopsThreads.remove(0);
        } catch (InterruptedException e) {
          LOG.error("Error waiting for thread join()", e);
        }
      }
    } finally {
      app.terminate();
    }
  }

  public static void main(String[] args) throws Exception {
    CmdLineOpts configuration = CmdLineOpts.createFromArgs(args);
    Main main = new Main(configuration);
    main.run();
    LOG.info("The sample app has finished");
  }
}