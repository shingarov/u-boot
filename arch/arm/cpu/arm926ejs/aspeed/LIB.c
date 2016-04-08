/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#define LIB_C
static const char ThisFile[] = "LIB.c";

#include "SWFUNC.H"

#include <common.h>
#include <command.h>

#include "LIB.H"
#include "TYPEDEF.H"

#ifdef USE_P2A
//------------------------------------------------------------
// PCI
//------------------------------------------------------------
ULONG ReadPCIReg (ULONG ulPCIConfigAddress, BYTE jOffest, ULONG ulMask)
{
#ifndef Windows
    OUTDWPORT(0xcf8, ulPCIConfigAddress + jOffest);

    return (((ULONG)INDWPORT(0xcfc)) & ulMask);
#else
    WRITE_PORT_ULONG((PULONG)0xcf8, ulPCIConfigAddress + jOffest);

    return (READ_PORT_ULONG((PULONG)0xcfc) & ulMask);
#endif
}

//------------------------------------------------------------
VOID WritePCIReg (ULONG ulPCIConfigAddress, BYTE jOffest, ULONG ulMask, ULONG ulData)
{
#ifndef Windows
    OUTDWPORT(0xcf8, ulPCIConfigAddress + jOffest);
    OUTDWPORT(0xcfc, (INDWPORT(0xcfc) & ulMask | ulData));
#else
    WRITE_PORT_ULONG((PULONG)0xcf8, ulPCIConfigAddress + jOffest);
    WRITE_PORT_ULONG((PULONG)0xcfc, (READ_PORT_ULONG((PULONG)0xcfc) & ulMask | ulData));
#endif
}

//------------------------------------------------------------
ULONG FindPCIDevice (USHORT usVendorID, USHORT usDeviceID, USHORT usBusType)
{
//Return: ulPCIConfigAddress
//usBusType: ACTIVE/PCI/AGP/PCI-E

    ULONG   Base[256];
    ULONG   ebx;
    USHORT  i;
    USHORT  j;

    for (i = 0; i < 256; i++) {
        Base[i] = 0x80000000 + 0x10000 * i;
    }

    if (usBusType == PCI)
    {
      ebx = 0x80000000;
    }
    else if (usBusType == PCIE)
    {
      ebx = 0x80020000;
    }
    else     // AGP and ACTIVE
    {
      ebx = 0x80010000;
    }

    if ( usBusType != ACTIVE )    //AGP, PCI, PCIE
    {
      for (i = 0; i < 32; i++)
      {
        ebx = ebx + (0x800);
        if (((USHORT)ReadPCIReg(ebx, 0, 0xffff) == usVendorID) && ((USHORT)(ReadPCIReg(ebx, 0, 0xffff0000) >> 16) == usDeviceID))
        {
          return ebx;
        }
      }
      return 0;
    }
    else     //ACTIVE
    {
      for (j = 0; j < 256; j++)
      {
        ebx = Base[j];
        for (i = 0; i < 32; i++)
        {
          ebx = ebx + (0x800);
          if (((USHORT)ReadPCIReg(ebx, 0, 0xffff) == usVendorID) && ((USHORT)(ReadPCIReg(ebx, 0, 0xffff0000) >> 16) == usDeviceID))
          {
            return ebx;
          }
        }
      }
      return 0;
    }
} // End ULONG FindPCIDevice (USHORT usVendorID, USHORT usDeviceID, USHORT usBusType)
#endif
