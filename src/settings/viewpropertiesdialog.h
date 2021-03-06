/***************************************************************************
 *   Copyright (C) 2006 by Peter Penz                                      *
 *   peter.penz@gmx.at                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

#ifndef VIEWPROPERTIESDIALOG_H
#define VIEWPROPERTIESDIALOG_H

#include "dolphin_export.h"

#include <QDialog>

class QCheckBox;
class KComboBox;
class QPushButton;
class QRadioButton;
class ViewProperties;
class DolphinView;

/**
 * @brief Dialog for changing the current view properties of a directory.
 *
 * It is possible to specify the view mode, the sorting order, whether hidden files
 * and previews should be shown. The properties can be assigned to the current folder,
 * or recursively to all sub folders.
 */
class DOLPHIN_EXPORT ViewPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewPropertiesDialog(DolphinView* dolphinView);
    virtual ~ViewPropertiesDialog();

public slots:
    void accept() Q_DECL_OVERRIDE;

private slots:
    void slotApply();
    void slotViewModeChanged(int index);
    void slotSortingChanged(int index);
    void slotSortOrderChanged(int index);
    void slotGroupedSortingChanged();
    void slotSortFoldersFirstChanged();
    void slotShowPreviewChanged();
    void slotShowHiddenFilesChanged();
    void markAsDirty(bool isDirty);
    void configureAdditionalInfo();

signals:
    void isDirtyChanged(bool isDirty);

private:
    void applyViewProperties();
    void loadSettings();

private:
    bool m_isDirty;
    DolphinView* m_dolphinView;
    ViewProperties* m_viewProps;

    KComboBox* m_viewMode;
    KComboBox* m_sortOrder;
    KComboBox* m_sorting;
    QCheckBox* m_sortFoldersFirst;
    QCheckBox* m_previewsShown;
    QCheckBox* m_showInGroups;
    QCheckBox* m_showHiddenFiles;
    QPushButton* m_additionalInfo;
    QRadioButton* m_applyToCurrentFolder;
    QRadioButton* m_applyToSubFolders;
    QRadioButton* m_applyToAllFolders;
    QCheckBox* m_useAsDefault;
};

#endif
