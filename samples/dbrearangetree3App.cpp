/***************************************************************
 * Name:      dbrearangetree3App.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2016-07-01
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "dbrearangetree3App.h"
#include "dbrearangetree3Main.h"

IMPLEMENT_APP(dbrearangetree3App);

bool dbrearangetree3App::OnInit()
{
    dbrearangetree3Frame* frame = new dbrearangetree3Frame(0L);
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();
    
    return true;
}
