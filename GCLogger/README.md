
把qt的输出信息写入日志文件（如qinfo，qdebug等），在新的线程写文件


使用方式：
	GCLogger* m_log;
    m_log = new GCLogger();
    m_log->openFile();
