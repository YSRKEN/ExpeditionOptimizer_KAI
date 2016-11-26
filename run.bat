echo off
prompt $G
:LOOP

set /P TABLE_TYPE="種類(戦艦少女は1、艦これは2、終了は0)："
if %TABLE_TYPE%==0 (goto EXIT)
set /P SUPPLY_1="燃料："
set /P SUPPLY_2="弾薬："
set /P SUPPLY_3="鋼材："
set /P SUPPLY_4="ボーキ："
set /P FLEETS="艦隊数："
set /P PAULING_TIME="チェックできる最小間隔："
set /P AUTO_SUPPLY="自然回復(あれば1、無ければ0)："
exp_opt %TABLE_TYPE% %SUPPLY_1% %SUPPLY_2% %SUPPLY_3% %SUPPLY_4% %FLEETS% %PAULING_TIME% %AUTO_SUPPLY%
echo.
goto LOOP
:EXIT
