'THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
'ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
'TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
'PARTICULAR PURPOSE.

'Copyright (C) 1987 - 2002.  Microsoft Corporation.  All rights reserved.

Dim Amount
AzBizRuleContext.BusinessRuleResult = FALSE
Amount = AzBizRuleContext.GetParameter("ExpAmount")
if Amount < 100 then AzBizRuleContext.BusinessRuleResult = TRUE