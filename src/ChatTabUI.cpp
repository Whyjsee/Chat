#include "../include/ChatTabUI.h"
#include "../include/Main.h"
#include <wx/image.h>
#include <wx/wfstream.h>

//#include "../capture.xpm"

ChatTabUI::ChatTabUI(wxNotebook* parentUI)
{
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxJPEGHandler);
    this->parentUI = parentUI;
}

void ChatTabUI::CreateTabUI(wxString& chatText,wxObject* mainFrame)
{

    panel = new wxPanel(parentUI);
    closeBtn = new wxButton(panel,wxID_CANCEL,_("关闭"));
    sendBtn = new wxButton(panel,wxID_OK,_("发送"));
    topSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    chatRTC = new wxRichTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,
                              wxSize(parentUI->GetClientSize().GetWidth(),parentUI->GetClientSize().GetHeight()*0.55),wxTE_MULTILINE | wxTE_RICH | wxTE_READONLY |wxTE_AUTO_URL);
    chatRTC->AppendText(chatText);
    sendRTC = new wxRichTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,
                                  wxSize(parentUI->GetClientSize().GetWidth(),parentUI->GetClientSize().GetHeight()*0.2),wxTE_MULTILINE | wxTE_RICH | wxTE_AUTO_URL);
    topSizer->Add(chatRTC,0,wxEXPAND | wxALL,10);
    topSizer->Add(sendRTC,0,wxEXPAND | wxALL,10);
    wxGridSizer* toolsSizer = new wxGridSizer(3,3,10);
    wxBitmap icon(_("capture.png"),wxBITMAP_TYPE_PNG);

    wxBitmapButton* captureBtn =new wxBitmapButton(panel,ID_CAPTURE_IMAGE,icon,wxDefaultPosition,wxSize(40,40),wxBU_AUTODRAW);
    toolsSizer->Add(captureBtn,0,wxALIGN_CENTER,0);

    buttonSizer->Add(toolsSizer,0,wxALL,10);
    buttonSizer->Add(closeBtn,0, wxALL,10);
    buttonSizer->Add(sendBtn,0, wxALL,10);
    //topSizer.Add(uttonSizer,0,wxALIGN_CENTER,0);
    ((MainFrame*)mainFrame)->Connect(ID_CAPTURE_IMAGE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainFrame::OnCaptureImageBtnClk);
    ((MainFrame*)mainFrame)->Connect(wxID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainFrame::OnCloseBtnClk);
    ((MainFrame*)mainFrame)->Connect(wxID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainFrame::OnSendBtnClk);
    topSizer->Add(buttonSizer);
    panel->SetSizer(topSizer);
}

void ChatTabUI::AppendChatText(wxString& who)
{
    //chatRTC->AppendText(text);
   // chatRTC->GetBuffer().Copy(sendRTC->GetBuffer());
   // chatRTC->GetBuffer() = sendRTC->GetBuffer();
	//	chatRTC->Newline();
	chatRTC->AppendText(_("\n") + who + _(":\n"));
	wxRichTextBuffer& buffer = chatRTC->GetBuffer();
	
	//buffer.InsertNewlineWithUndo(chatRTC->GetLastPosition(),sendRTC,wxRICHTEXT_INSERT_INTERACTIVE);
	wxRichTextObjectList& chatObjectList = buffer.GetChildren();
	wxRichTextObjectList& sendObjectList = sendRTC->GetBuffer().GetChildren();
	
	wxRichTextObjectList::compatibility_iterator node = sendObjectList.GetFirst();

	while(node)
	{
	    wxRichTextObject* child = node->GetData();
		
        wxRichTextObject* newChild = child->Clone();
				
        newChild->SetParent(&buffer);
		chatObjectList.Append(newChild);
		node = node->GetNext();

	}
	
    chatRTC->GetBuffer().Invalidate();
	chatRTC->RecreateBuffer();
    chatRTC->Refresh();
	chatRTC->Update();//make the Last Position right
	buffer.UpdateRanges();
	wxTextPos pos = sendRTC->GetLastPosition();
	wxTextPos chatPos = chatRTC->GetLastPosition();
	chatRTC->ShowPosition(chatPos + pos);
	
}

wxString ChatTabUI::GetSendText()
{
    wxString text = sendRTC->GetValue();

    //sendRTC->GetPh

   // sendRTC->SetValue("");
	//sendRTC->SetFocus();
    return text;
}
void ChatTabUI::AppendImageToSendTextctrl(wxString& path)
{
    wxImage image(path,wxBITMAP_TYPE_PNG);
    if(image.Ok())
    {
        sendRTC->WriteImage(image);

    }
}
