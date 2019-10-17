// Textbox.h: Definition of the Textbox class
//
//////////////////////////////////////////////////////////////////////
#pragma once
#if !defined(AFX_TEXTBOX_H__ED083DAA_135F_4420_8701_F99205C725DA__INCLUDED_)
#define AFX_TEXTBOX_H__ED083DAA_135F_4420_8701_F99205C725DA__INCLUDED_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// Textbox

class TextboxData
{
public:
   Vertex2D m_v1, m_v2;
   float m_fontsize;
   COLORREF m_backcolor;
   COLORREF m_fontcolor;
   float m_intensity_scale;
   char sztext[MAXSTRING];
   TimerDataRoot m_tdr;
   TextAlignment m_talign;
   bool m_transparent;
   bool m_visible;
   bool m_isDMD;
};

class Textbox :
   public CComObjectRootEx<CComSingleThreadModel>,
   public IDispatchImpl<ITextbox, &IID_ITextbox, &LIBID_VPinballLib>,
   //public ISupportErrorInfo,
   //public CComObjectRoot,
   public CComCoClass<Textbox, &CLSID_Textbox>,
   public EventProxy<Textbox, &DIID_ITextboxEvents>,
   public IConnectionPointContainerImpl<Textbox>,
   public IProvideClassInfo2Impl<&CLSID_Textbox, &DIID_ITextboxEvents, &LIBID_VPinballLib>,
   public ISelect,
   public IEditable,
   public IScriptable,
   public IFireEvents,
   public Hitable
{
public:
   Textbox();
   virtual ~Textbox();
   BEGIN_COM_MAP(Textbox)
      COM_INTERFACE_ENTRY(IDispatch)
      COM_INTERFACE_ENTRY(ITextbox)
      COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
      COM_INTERFACE_ENTRY(IProvideClassInfo)
      COM_INTERFACE_ENTRY(IProvideClassInfo2)
      //COM_INTERFACE_ENTRY(ISupportErrorInfo)
   END_COM_MAP()
   //DECLARE_NOT_AGGREGATABLE(Textbox) 
   // Remove the comment from the line above if you don't want your object to 
   // support aggregation.

   BEGIN_CONNECTION_POINT_MAP(Textbox)
      CONNECTION_POINT_ENTRY(DIID_ITextboxEvents)
   END_CONNECTION_POINT_MAP()

   STANDARD_EDITABLE_DECLARES(Textbox, eItemTextbox, TEXTBOX, 2)

   virtual void GetDialogPanes(vector<PropertyPane*> &pvproppane);

   virtual void MoveOffset(const float dx, const float dy);
   virtual void SetObjectPos();
   // Multi-object manipulation
   virtual Vertex2D GetCenter() const { return m_d.m_v1; }
   virtual void PutCenter(const Vertex2D& pv);
   virtual ItemTypeEnum HitableGetItemType() const { return eItemTextbox; }

   DECLARE_REGISTRY_RESOURCEID(IDR_TEXTBOX)
   // ISupportsErrorInfo
   STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

   virtual void WriteRegDefaults();

private:
   void PreRenderText();

   PinTable *m_ptable;

   TextboxData m_d;

   BaseTexture *m_texture;

   IFont *m_pIFont;

   IFont *m_pIFontPlay; // Our font, scaled to match play window resolution

   // ITextbox
public:
   STDMETHOD(get_IsTransparent)(/*[out, retval]*/ VARIANT_BOOL *pVal);
   STDMETHOD(put_IsTransparent)(/*[in]*/ VARIANT_BOOL newVal);
   STDMETHOD(get_DMD)(/*[out, retval]*/ VARIANT_BOOL *pVal);
   STDMETHOD(put_DMD)(/*[in]*/ VARIANT_BOOL newVal);
   STDMETHOD(get_Visible)(/*[out, retval]*/ VARIANT_BOOL *pVal);
   STDMETHOD(put_Visible)(/*[in]*/ VARIANT_BOOL newVal);
   STDMETHOD(get_Alignment)(/*[out, retval]*/ TextAlignment *pVal);
   STDMETHOD(put_Alignment)(/*[in]*/ TextAlignment newVal);
   STDMETHOD(get_Y)(/*[out, retval]*/ float *pVal);
   STDMETHOD(put_Y)(/*[in]*/ float newVal);
   STDMETHOD(get_X)(/*[out, retval]*/ float *pVal);
   STDMETHOD(put_X)(/*[in]*/ float newVal);
   STDMETHOD(get_IntensityScale)(/*[out, retval]*/ float *pVal);
   STDMETHOD(put_IntensityScale)(/*[in]*/ float newVal);
   STDMETHOD(get_Height)(/*[out, retval]*/ float *pVal);
   STDMETHOD(put_Height)(/*[in]*/ float newVal);
   STDMETHOD(get_Width)(/*[out, retval]*/ float *pVal);
   STDMETHOD(put_Width)(/*[in]*/ float newVal);
   STDMETHOD(get_Font)(/*[out, retval]*/ IFontDisp **pVal);
   STDMETHOD(put_Font)(/*[in]*/ IFontDisp *newVal);
   STDMETHOD(putref_Font)(IFontDisp* pFont);
   STDMETHOD(get_Text)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(put_Text)(/*[in]*/ BSTR newVal);
   STDMETHOD(get_FontColor)(/*[out, retval]*/ OLE_COLOR *pVal);
   STDMETHOD(put_FontColor)(/*[in]*/ OLE_COLOR newVal);
   STDMETHOD(get_BackColor)(/*[out, retval]*/ OLE_COLOR *pVal);
   STDMETHOD(put_BackColor)(/*[in]*/ OLE_COLOR newVal);
};

#endif // !defined(AFX_TEXTBOX_H__ED083DAA_135F_4420_8701_F99205C725DA__INCLUDED_)
