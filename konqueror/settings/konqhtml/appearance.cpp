
#include <QFontDatabase>
#include <QLabel>
#include <QLayout>
#include <q3groupbox.h>

#include <dcopclient.h>

#include <kapplication.h>
#include <kcharsets.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kdialog.h>
#include <kfontcombo.h>
#include <kglobal.h>
#include <khtmldefaults.h>
#include <klocale.h>
#include <knuminput.h>

#if defined Q_WS_X11 && !defined K_WS_QTONLY
#include <X11/Xlib.h>
#endif


#include "appearance.moc"

KAppearanceOptions::KAppearanceOptions(KConfig *config, QString group, KInstance *inst, QWidget *parent)
    : KCModule( inst, parent ), m_pConfig(config), m_groupname(group),
      fSize( 10 ), fMinSize( HTML_DEFAULT_MIN_FONT_SIZE )

{
  setQuickHelp( i18n("<h1>Konqueror Fonts</h1>On this page, you can configure "
              "which fonts Konqueror should use to display the web "
              "pages you view."));

  QString wtstr;
  //initialise fonts list otherwise it crashs
    while (fonts.count() < 7)
       fonts.append(QString());

  QGridLayout *lay = new QGridLayout(this);
  lay->setSpacing(KDialog::spacingHint());
  lay->setMargin(0);
  int r = 0;
  int E = 0, M = 1, W = 3; //CT 3 (instead 2) allows smaller color buttons

  Q3GroupBox* gb = new Q3GroupBox( 1, Qt::Horizontal, i18n("Font Si&ze"), this );
  lay->addWidget(gb, r, E, 1, W- E+1);

  gb->setWhatsThis( i18n("This is the relative font size Konqueror uses to display web sites.") );

  m_minSize = new KIntNumInput( fMinSize, gb );
  m_minSize->setLabel( i18n( "M&inimum font size:" ) );
  m_minSize->setRange( 2, 30 );
  connect( m_minSize, SIGNAL( valueChanged( int ) ), this, SLOT( slotMinimumFontSize( int ) ) );
  connect( m_minSize, SIGNAL( valueChanged( int ) ), this, SLOT( changed() ) );
  m_minSize->setWhatsThis( i18n( "Konqueror will never display text smaller than "
                                    "this size,<br>overriding any other settings" ) );

  m_MedSize = new KIntNumInput( m_minSize, fSize, gb );
  m_MedSize->setLabel( i18n( "&Medium font size:" ) );
  m_MedSize->setRange( 2, 30 );
  connect( m_MedSize, SIGNAL( valueChanged( int ) ), this, SLOT( slotFontSize( int ) ) );
  connect( m_MedSize, SIGNAL( valueChanged( int ) ), this, SLOT( changed() ) );
  m_MedSize->setWhatsThis(
                   i18n("This is the relative font size Konqueror uses "
                        "to display web sites.") );

  QStringList emptyList;

  QLabel* label = new QLabel( i18n("S&tandard font:"), this );
  lay->addWidget( label , ++r, E);

  m_pFonts[0] = new KFontCombo( emptyList, this );

  label->setBuddy( m_pFonts[0] );
  lay->addWidget(m_pFonts[0], r, M, 1, W- M+1);

  wtstr = i18n("This is the font used to display normal text in a web page.");
  label->setWhatsThis( wtstr );
  m_pFonts[0]->setWhatsThis( wtstr );

  connect( m_pFonts[0], SIGNAL( activated(const QString&) ),
	   SLOT( slotStandardFont(const QString&) ) );
  connect( m_pFonts[0], SIGNAL( activated(const QString&) ),
	   SLOT(changed() ) );
  connect( m_pFonts[0]->lineEdit(), SIGNAL( textChanged(const QString&) ),
	   SLOT( slotStandardFont(const QString&) ) );
  connect( m_pFonts[0], SIGNAL( textChanged(const QString&) ),
	   SLOT(changed() ) );

  label = new QLabel( i18n( "&Fixed font:"), this );
  lay->addWidget( label, ++r, E );

  m_pFonts[1] = new KFontCombo( emptyList, this );

  label->setBuddy( m_pFonts[1] );
  lay->addWidget(m_pFonts[1], r, M, 1, W- M+1);

  wtstr = i18n("This is the font used to display fixed-width (i.e. non-proportional) text.");
  label->setWhatsThis( wtstr );
  m_pFonts[1]->setWhatsThis( wtstr );

  connect( m_pFonts[1], SIGNAL( activated(const QString&) ),
	   SLOT( slotFixedFont(const QString&) ) );
  connect( m_pFonts[1], SIGNAL( activated(const QString&) ),
	   SLOT(changed() ) );
  connect( m_pFonts[1]->lineEdit(), SIGNAL( textChanged(const QString&) ),
	   SLOT( slotFixedFont(const QString&) ) );
  connect( m_pFonts[1], SIGNAL( textChanged(const QString&) ),
	   SLOT(changed() ) );

  label = new QLabel( i18n( "S&erif font:" ), this );
  lay->addWidget( label, ++r, E );

  m_pFonts[2] = new KFontCombo( emptyList, this );

  label->setBuddy( m_pFonts[2] );
  lay->addWidget( m_pFonts[2], r, M, 1, W - M+1);

  wtstr= i18n( "This is the font used to display text that is marked up as serif." );
  label->setWhatsThis( wtstr );
  m_pFonts[2]->setWhatsThis( wtstr );

  connect( m_pFonts[2], SIGNAL( activated( const QString& ) ),
	   SLOT( slotSerifFont( const QString& ) ) );
  connect( m_pFonts[2], SIGNAL( activated( const QString& ) ),
	   SLOT( changed() ) );
  connect( m_pFonts[2]->lineEdit(), SIGNAL( textChanged(const QString&) ),
	   SLOT( slotSerifFont(const QString&) ) );
  connect( m_pFonts[2], SIGNAL( textChanged(const QString&) ),
	   SLOT(changed() ) );

  label = new QLabel( i18n( "Sa&ns serif font:" ), this );
  lay->addWidget( label, ++r, E );

  m_pFonts[3] = new KFontCombo( emptyList, this );

  label->setBuddy( m_pFonts[3] );
  lay->addWidget( m_pFonts[3], r, M, 1, W - M+1);

  wtstr= i18n( "This is the font used to display text that is marked up as sans-serif." );
  label->setWhatsThis( wtstr );
  m_pFonts[3]->setWhatsThis( wtstr );

  connect( m_pFonts[3], SIGNAL( activated( const QString& ) ),
	   SLOT( slotSansSerifFont( const QString& ) ) );
  connect( m_pFonts[3], SIGNAL( activated( const QString& ) ),
	   SLOT( changed() ) );
  connect( m_pFonts[3]->lineEdit(), SIGNAL( textChanged(const QString&) ),
	   SLOT( slotSansSerifFont(const QString&) ) );
  connect( m_pFonts[3], SIGNAL( textChanged(const QString&) ),
	   SLOT(changed() ) );


  label = new QLabel( i18n( "C&ursive font:" ), this );
  lay->addWidget( label, ++r, E );

  m_pFonts[4] = new KFontCombo( emptyList, this );

  label->setBuddy( m_pFonts[4] );
  lay->addWidget( m_pFonts[4], r, M, 1, W - M+1);

  wtstr= i18n( "This is the font used to display text that is marked up as italic." );
  label->setWhatsThis( wtstr );
  m_pFonts[4]->setWhatsThis( wtstr );

  connect( m_pFonts[4], SIGNAL( activated( const QString& ) ),
	   SLOT( slotCursiveFont( const QString& ) ) );
  connect( m_pFonts[4], SIGNAL( activated( const QString& ) ),
	   SLOT( changed() ) );
  connect( m_pFonts[4]->lineEdit(), SIGNAL( textChanged(const QString&) ),
	   SLOT( slotCursiveFont(const QString&) ) );
  connect( m_pFonts[4], SIGNAL( textChanged(const QString&) ),
	   SLOT(changed() ) );


  label = new QLabel( i18n( "Fantas&y font:" ), this );
  lay->addWidget( label, ++r, E );

  m_pFonts[5] = new KFontCombo( emptyList, this );

  label->setBuddy( m_pFonts[5] );
  lay->addWidget( m_pFonts[5], r, M, 1, W-M+1 );

  wtstr= i18n( "This is the font used to display text that is marked up as a fantasy font." );
  label->setWhatsThis( wtstr );
  m_pFonts[5]->setWhatsThis( wtstr );

  connect( m_pFonts[5], SIGNAL( activated( const QString& ) ),
	   SLOT( slotFantasyFont( const QString& ) ) );
  connect( m_pFonts[5], SIGNAL( activated( const QString& ) ),
	   SLOT( changed() ) );
  connect( m_pFonts[5]->lineEdit(), SIGNAL( textChanged(const QString&) ),
	   SLOT( slotFantasyFont(const QString&) ) );
  connect( m_pFonts[5], SIGNAL( textChanged(const QString&) ),
	   SLOT(changed() ) );


  label = new QLabel( i18n( "Font &size adjustment for this encoding:" ), this );
  lay->addWidget( label, ++r, M );

  m_pFontSizeAdjust = new QSpinBox( this );
  m_pFontSizeAdjust->setRange( -5, 5 );
  m_pFontSizeAdjust->setSingleStep( 1 );
  label->setBuddy( m_pFontSizeAdjust );
  lay->addWidget( m_pFontSizeAdjust, r, M+1, 1, W-M+2 );

  connect( m_pFontSizeAdjust, SIGNAL( valueChanged( int ) ),
	   SLOT( slotFontSizeAdjust( int ) ) );
  connect( m_pFontSizeAdjust, SIGNAL( valueChanged( int ) ),
	   SLOT( changed() ) );

  label = new QLabel( i18n( "Default encoding:"), this );
  //++r;
  //lay->addWidget( label, r, E, 1, 2 );
  lay->addWidget( label, ++r, E);

  m_pEncoding = new QComboBox( this );
  label->setBuddy( m_pEncoding );
  m_pEncoding->setEditable( false );
  encodings = KGlobal::charsets()->availableEncodingNames();
  encodings.prepend(i18n("Use Language Encoding"));
  m_pEncoding->addItems( encodings );
  lay->addWidget(m_pEncoding,r, M, 1, W- M+1);

  wtstr = i18n( "Select the default encoding to be used; normally, you will be fine with 'Use language encoding' "
	       "and should not have to change this.");
  label->setWhatsThis( wtstr );
  m_pEncoding->setWhatsThis( wtstr );

  connect( m_pEncoding, SIGNAL( activated(const QString& ) ),
	   SLOT( slotEncoding(const QString&) ) );
  connect( m_pEncoding, SIGNAL( activated(const QString& ) ),
	   SLOT( changed() ) );

  ++r; lay->setRowStretch(r, 8);

  QFontDatabase db;

  m_families = db.families();

  m_pFonts[0]->setFonts( m_families );
  m_pFonts[1]->setFonts( m_families );
  m_pFonts[2]->setFonts( m_families );
  m_pFonts[3]->setFonts( m_families );
  m_pFonts[4]->setFonts( m_families );
  m_pFonts[5]->setFonts( m_families );

  load();
}

KAppearanceOptions::~KAppearanceOptions()
{
delete m_pConfig;
}

void KAppearanceOptions::slotFontSize( int i )
{
    fSize = i;
    if ( fSize < fMinSize ) {
        m_minSize->setValue( fSize );
        fMinSize = fSize;
    }
}


void KAppearanceOptions::slotMinimumFontSize( int i )
{
    fMinSize = i;
    if ( fMinSize > fSize ) {
        m_MedSize->setValue( fMinSize );
        fSize = fMinSize;
    }
}


void KAppearanceOptions::slotStandardFont(const QString& n )
{
    fonts[0] = n;
}


void KAppearanceOptions::slotFixedFont(const QString& n )
{
    fonts[1] = n;
}


void KAppearanceOptions::slotSerifFont( const QString& n )
{
    fonts[2] = n;
}


void KAppearanceOptions::slotSansSerifFont( const QString& n )
{
    fonts[3] = n;
}


void KAppearanceOptions::slotCursiveFont( const QString& n )
{
    fonts[4] = n;
}


void KAppearanceOptions::slotFantasyFont( const QString& n )
{
    fonts[5] = n;
}

void KAppearanceOptions::slotFontSizeAdjust( int value )
{
    fonts[6] = QString::number( value );
}

void KAppearanceOptions::slotEncoding(const QString& n)
{
    encodingName = n;
}

void KAppearanceOptions::load()
{
    KConfig khtmlrc("khtmlrc", true, false);
#define SET_GROUP(x) m_pConfig->setGroup(x); khtmlrc.setGroup(x)
#define READ_NUM(x,y) m_pConfig->readEntry(x, khtmlrc.readEntry(x, y))
#define READ_ENTRY(x,y) m_pConfig->readEntry(x, khtmlrc.readEntry(x, y))
#define READ_LIST(x) m_pConfig->readEntry(x, khtmlrc.readEntry(x, QStringList() ))

    SET_GROUP(m_groupname);
    fSize = READ_NUM( "MediumFontSize", 12 );
    fMinSize = READ_NUM( "MinimumFontSize", HTML_DEFAULT_MIN_FONT_SIZE );
    if (fSize < fMinSize)
      fSize = fMinSize;

    defaultFonts = QStringList();
    defaultFonts.append( READ_ENTRY( "StandardFont", KGlobalSettings::generalFont().family() ) );
    defaultFonts.append( READ_ENTRY( "FixedFont", KGlobalSettings::fixedFont().family() ) );
    defaultFonts.append( READ_ENTRY( "SerifFont", HTML_DEFAULT_VIEW_SERIF_FONT ) );
    defaultFonts.append( READ_ENTRY( "SansSerifFont", HTML_DEFAULT_VIEW_SANSSERIF_FONT ) );
    defaultFonts.append( READ_ENTRY( "CursiveFont", HTML_DEFAULT_VIEW_CURSIVE_FONT ) );
    defaultFonts.append( READ_ENTRY( "FantasyFont", HTML_DEFAULT_VIEW_FANTASY_FONT ) );
    defaultFonts.append( QString("0") ); // default font size adjustment

    if (m_pConfig->hasKey("Fonts"))
       fonts = m_pConfig->readEntry( "Fonts" , QStringList() );
    else
       fonts = khtmlrc.readEntry( "Fonts" , QStringList() );
    while (fonts.count() < 7)
       fonts.append(QString());

    encodingName = READ_ENTRY( "DefaultEncoding", QString() );
    //kDebug(0) << "encoding = " << encodingName << endl;

    updateGUI();
    emit changed(false);
}

void KAppearanceOptions::defaults()
{
    bool old = m_pConfig->readDefaults();
    m_pConfig->setReadDefaults(true);
    load();
    m_pConfig->setReadDefaults(old);
}

void KAppearanceOptions::updateGUI()
{
    //kDebug() << "KAppearanceOptions::updateGUI " << charset << endl;
    for ( int f = 0; f < 6; f++ ) {
        QString ff = fonts[f];
        if (ff.isEmpty())
           ff = defaultFonts[f];
        m_pFonts[f]->setCurrentFont(ff);
    }

    int i = 0;
    for ( QStringList::Iterator it = encodings.begin(); it != encodings.end(); ++it, ++i )
        if ( encodingName == *it )
            m_pEncoding->setCurrentIndex( i );
    if(encodingName.isEmpty())
        m_pEncoding->setCurrentIndex( 0 );
    m_pFontSizeAdjust->setValue( fonts[6].toInt() );
    m_MedSize->blockSignals(true);
    m_MedSize->setValue( fSize );
    m_MedSize->blockSignals(false);
    m_minSize->blockSignals(true);
    m_minSize->setValue( fMinSize );
    m_minSize->blockSignals(false);
}

void KAppearanceOptions::save()
{
    m_pConfig->setGroup(m_groupname);
    m_pConfig->writeEntry( "MediumFontSize", fSize );
    m_pConfig->writeEntry( "MinimumFontSize", fMinSize );
    m_pConfig->writeEntry( "Fonts", fonts );

    // If the user chose "Use language encoding", write an empty string
    if (encodingName == i18n("Use Language Encoding"))
        encodingName = "";
    m_pConfig->writeEntry( "DefaultEncoding", encodingName );
    m_pConfig->sync();

  QByteArray data;
  if ( !kapp->dcopClient()->isAttached() )
    kapp->dcopClient()->attach();
  kapp->dcopClient()->send( "konqueror*", "KonquerorIface", "reparseConfiguration()", data );

  emit changed(false);
}

