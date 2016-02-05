@echo 获取迅雷VIP帐号
@cd %~dp0bin/
@echo 请稍等正在获取数据中.............
@xlvip.exe  > %tmp%\ok.txt
@start %tmp%\ok.txt