/*Browser*/

BAS_SolveRecaptcha_Method = <%= method %>;
BAS_SolveRecaptcha_Rucaptcha = <%= rucaptcha %>;
BAS_SolveRecaptcha_Serverurl = <%= serverurl %>;
BAS_SolveRecaptcha_Query = <%= query %>;
BAS_SolveRecaptcha_Task_List = <%= tasklist %>;
BAS_SolveRecaptcha_Disableemulation = <%= disableemulation %>;
BAS_SolveRecaptcha_SendProxy = (<%= sendproxy %>) == "true";

_SELECTOR = BAS_SolveRecaptcha_Query;
BAS_SolveRecaptcha_Waiter = function()
{
  <%= waiter %>
};
BAS_SolveRecaptcha_Path = function(){return (<%= path %>)};
BAS_SolveRecaptcha_TimeToSolve = <%= timestosolve %>;

_call(BAS_SolveRecaptcha,null)!