echo off
prompt $G
:LOOP

set /P TABLE_TYPE="���(��͏�����1�A�͂����2�A�I����0)�F"
if %TABLE_TYPE%==0 (goto EXIT)
set /P SUPPLY_1="�R���F"
set /P SUPPLY_2="�e��F"
set /P SUPPLY_3="�|�ށF"
set /P SUPPLY_4="�{�[�L�F"
set /P FLEETS="�͑����F"
set /P PAULING_TIME="�`�F�b�N�ł���ŏ��Ԋu�F"
set /P AUTO_SUPPLY="���R��(�����1�A�������0)�F"
exp_opt %TABLE_TYPE% %SUPPLY_1% %SUPPLY_2% %SUPPLY_3% %SUPPLY_4% %FLEETS% %PAULING_TIME% %AUTO_SUPPLY%
echo.
goto LOOP
:EXIT
